#include <arpa/inet.h>
#include <cassert>
#include <ctime>
#include <iostream>
#include <mutex>
#include <functional>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>

#include "Agent.hpp"
#include "Codes.hpp"
#include "Serialize.hpp"
#include "TypeUtil.hpp"

#define STREAM_SEND(BUF, SIZE) \
 ( \
   { \
    sent_ttl = 0; sent = 0; \
    while (sent_ttl < SIZE) { \
      sent = stream->send((BUF), (SIZE)); \
      if (sent < 0) { \
        std::cout << "Stream send error" << std::endl; \
        break; \
      } \
      sent_ttl += sent; \
    } \
  } \
 )
#define STREAM_RECV(BUF, SIZE) \
 ( \
   { \
    recvd_ttl = 0; recvd = 0; \
    while (recvd_ttl < SIZE) { \
      recvd = stream->receive((BUF), (SIZE)); \
      if (recvd < 0) { \
        std::cout << "Stream recv error" << std::endl; \
        break; \
      } \
      recvd_ttl += recvd; \
    } \
  } \
 )

class ObjEntry {
 public:
  AgentObj* agentObj;
  bool server_deleted; // delete has been called on corresponding server obj
  time_t renewed_time;
  ObjEntry(AgentObj* obj_, bool server_deleted_, time_t renewed_time_)
    : agentObj(obj_), server_deleted(server_deleted_),
      renewed_time(renewed_time_) {}
  ~ObjEntry() {
    delete agentObj;
  }
};

void _run(std::mutex& mtx,
    std::unordered_map<std::string, ObjEntry*>& name_to_obj,
    volatile bool& should_exit, int port, std::string& ip_addr) {
  TCPAcceptor* acceptor = new TCPAcceptor(port, ip_addr.c_str());
  if (acceptor->start() != 0) {
    std::cout << "Agent failed to start TCP acceptor" << std::endl;
    mtx.unlock();
    exit(1);
  }
  std::cout << "Agent started at " << ip_addr << ":" << port << std::endl;

  ssize_t sent_ttl; ssize_t sent; ssize_t recvd; ssize_t recvd_ttl;
  uint iteration = 0;
  while (!should_exit) {
    ++iteration;
    if (iteration % 2 == 0) {
      std::cout << "garbage collection initiated" << std::endl;
      time_t cur_time = time(NULL);
      for (auto it = name_to_obj.begin(); it != name_to_obj.end(); ) {
        std::cout << it->first << " deleted by server? " << it->second->server_deleted << std::endl;
        if (it->second->server_deleted) {
          double diff_time = difftime(cur_time, it->second->renewed_time);
          std::cout << "difftime: " << diff_time << std::endl;
          if (diff_time > 10.0) {
            name_to_obj.erase(it++);
            std::cout << "deleted " << it->first << " and its entry" << std::endl;
            continue;
          }
        }
        ++it;
      }
    }
    mtx.unlock();
    TCPStream* stream = acceptor->accept();
    if (stream == NULL) { goto err_exit; }

    char message_type;
    STREAM_RECV(&message_type, sizeof(message_type));
    if (recvd < 0) { goto cleanup_stream; }

//      std::cout << "message_type " << (int)message_type << std::endl;
    switch (static_cast<MessageCode>(message_type)) {
      case MessageCode::get_type: {
        std::cout << "get_type" << std::endl;

        uint32_t name_len;
        STREAM_RECV((char*)&name_len, sizeof(name_len));
        if (recvd < 0) { goto cleanup_stream; }
        name_len = ntohl(name_len);
//          std::cout << "name_len " << name_len << std::endl;

        char* name = new char[name_len+1];
        STREAM_RECV(name, name_len);
        if (recvd < 0) { goto cleanup_name; }
        name[name_len] = '\0';
//          std::cout << "name " << name << std::endl;

        mtx.lock(); {
        auto it = name_to_obj.find(name);
        if (it == name_to_obj.end()) {
          uint32_t type_len = 0;
          STREAM_SEND((char*)&type_len, sizeof(type_len));
          if (sent < 0) { goto cleanup_get_type_response; }
        } else { // found NetObj for name
          it->second->renewed_time = time(NULL); // GC
          std::string type = typeid(*(it->second->agentObj)).name();
          uint32_t type_len = htonl(type.size());
          STREAM_SEND((char*)&type_len, sizeof(type_len));
          if (sent < 0) { goto cleanup_get_type_response; }

          STREAM_SEND(type.c_str(), type.size());
          if (sent < 0) { goto cleanup_get_type_response; }
          std::cout << "sent " << name << " of type " << type << std::endl;
        }
        }
cleanup_get_type_response: mtx.unlock();
cleanup_name: delete[] name;
        break;
      }
      case MessageCode::invoke: {
        std::cout << "invoke" << std::endl;
        uint32_t buf_size;
        STREAM_RECV((char*)&buf_size, sizeof(buf_size));
        if (recvd < 0) { goto cleanup_stream; }
        buf_size = ntohl(buf_size);

        std::cout << "expecting buf of size " << buf_size << std::endl;
        char* buf = new char[buf_size];
        STREAM_RECV(buf, buf_size);
        if (recvd < 0) { goto cleanup_buf; }
        std::cout << "got buf of size " << buf_size << std::endl;

        {
        std::string name;
        size_t start = Serialize::unpack(buf, 0, name);
        std::cout << "invoke requested for " << name << std::endl;

        mtx.lock();
        auto it = name_to_obj.find(name);
        it->second->renewed_time = time(NULL); // GC
        // TODO: check existence
        mtx.unlock();
        char* res_buf; uint32_t res_buf_size;
        (it->second->agentObj)->dispatch(buf+start, res_buf, res_buf_size);
        if (res_buf == nullptr) { goto cleanup_buf; }

        uint32_t res_buf_size_wire = htonl(res_buf_size);
        STREAM_SEND((char*)&res_buf_size_wire, sizeof(res_buf_size_wire));
        if (sent < 0) { goto cleanup_res_buf; }

        STREAM_SEND(res_buf, res_buf_size);
        if (sent < 0) { goto cleanup_res_buf; }
cleanup_res_buf: delete[] res_buf;
        }

cleanup_buf: delete[] buf;
        break;
      }
      default: {
        std::cout << "no valid message type received" << std::endl;
        break;
      }
    }
cleanup_stream: delete stream;
err_exit:
    mtx.lock();
  }
  // TODO thread cleanup
}
void Agent::Initialize(int port, std::string ip_addr) { // don't call more than once
  _should_exit = false;
  _mtx.lock();
  std::thread(_run, std::ref(_mtx), std::ref(_name_to_obj),
      std::ref(_should_exit), port, std::ref(ip_addr)).detach();
}
void Agent::Exit() { // assumes Initialize was called, don't call more than once
  _mtx.lock();
  _should_exit = true;
  _mtx.unlock();
}
ClientObj* Agent::Import(std::string name, int port, std::string ip_addr) {
  char message_type; uint32_t name_len;
  ssize_t sent_ttl; ssize_t sent; ssize_t recvd; ssize_t recvd_ttl;
  ClientObj* return_res = NULL;

  TCPConnector* connector = new TCPConnector();
  TCPStream* stream = connector->connect(ip_addr.c_str(), port);
  if (stream == NULL) { goto cleanup_connector; }

  message_type = static_cast<char>(MessageCode::get_type); // char, so no endian worry
  STREAM_SEND((const char*)&message_type, sizeof(message_type));
  if (sent < 0) { goto cleanup_connector; }

  name_len = htonl(name.size());
  STREAM_SEND((const char*)&name_len, sizeof(name_len));
  if (sent < 0) { goto cleanup_connector; }

  STREAM_SEND(name.c_str(), name.size());
  if (sent < 0) { goto cleanup_connector; }

  uint32_t type_len;
  STREAM_RECV((char*)&type_len, sizeof(type_len));
  if (recvd < 0) { goto cleanup_connector; }
  type_len = ntohl(type_len);
  std::cout << "type_len " << type_len << std::endl;

  if (type_len == 0) {
    std::cout << "No NetObj named " << name << " found" << std::endl;
    goto cleanup_connector;
  }

  {
  char* type = new char[type_len+1];
  STREAM_RECV(type, type_len);
  if (recvd < 0) { goto cleanup_type; }
  type[type_len] = '\0';
  std::cout << "Agent::Import " << name << " of type " << type << std::endl;
  return_res = _type_util.getClientObjFromAgentName(type, name, ip_addr, port);
cleanup_type: delete[] type;
  }
cleanup_connector: delete connector;
  return return_res;
}

void Agent::Export(const std::string name, NetObj* netObj) {
  _mtx.lock();
  if (!(_name_to_obj.find(name) == _name_to_obj.end())) {
    _mtx.unlock();
    throw std::runtime_error(name + "already assigned");
  }
  AgentObj* agentObj = _type_util.getAgentObjForServerObj(netObj, name, this);
  _name_to_obj[name] = new ObjEntry(agentObj, false, time(NULL));
//    std::cout << "put " << typeid(*agentObj).name() << " " << name << std::endl;
  _mtx.unlock();
}
void Agent::PrintExported() {
  _mtx.lock();
  for (auto it = _name_to_obj.begin(); it != _name_to_obj.end(); ++it) {
    std::cout << typeid(*(it->second->agentObj)).name() << " " << it->first
      << std::endl;
  }
  _mtx.unlock();
}
// mark obj with name as deleted by server (precondition for GC)
void Agent::mark_obj_deleted(std::string name) {
  _mtx.lock();
  if (_name_to_obj.find(name) != _name_to_obj.end()) {
    _name_to_obj[name]->server_deleted = true;
    std::cout << name << " marked as deleted by server" << std::endl;
  }
  _mtx.unlock();
}

RPCRequest Agent::rpc_receive(TCPStream* stream) {
  std::string buffer;
  receive_packet(stream, buffer);

  RPCRequest request = RPCRequest::load_packet(buffer);
  if (_debugMode) {
      std::cerr << "Request:" << request.to_str() << "\n";
  }
  return request;
}
