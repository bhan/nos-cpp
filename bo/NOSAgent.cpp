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

#include "NOSAgent.hpp"
#include "Codes.hpp"
#include "RPCResponse.hpp"
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

static void _run(std::mutex& mtx, NOSAgent* agent,
    std::unordered_map<std::string, ObjEntry*>& name_to_obj,
    volatile bool& should_exit, std::string& address, uint32_t port) {
  TCPAcceptor* acceptor = new TCPAcceptor(port, address.c_str());
  if (acceptor->start() != 0) {
    std::cout << "NOSAgent failed to start TCP acceptor" << std::endl;
    mtx.unlock();
    ::exit(1);
  }
  std::cout << "NOSAgent started at " << address << ":" << port << std::endl;

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
    RPCRequest request;
    TCPStream* stream = acceptor->accept();
    if (stream == NULL) { goto err_exit; }

    request = agent->rpc_receive(stream);
    switch (static_cast<MessageCode>(request.Type)) {
      case MessageCode::get_type: {
        std::cout << "get_type" << std::endl;
        mtx.lock();
        auto it = name_to_obj.find(request.ObjectID);
        RPCResponse response;
        if (it == name_to_obj.end()) {
          response.Code = ServerCode::OBJECT_NOT_FOUND;
        } else { // found NetObj for name
          it->second->renewed_time = time(NULL); // GC
          response.Code = ServerCode::OK;
          std::string type = typeid(*(it->second->agentObj)).name();
          response.Body = type;
        }
        agent->send_packet(stream, response.packet());
        mtx.unlock();
        break;
      }
      case MessageCode::invoke: {
/*        std::cout << "invoke" << std::endl;
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
*/        break;
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
void NOSAgent::initialize(std::string address, uint32_t port) { // don't call more than once
  _should_exit = false;
  _mtx.lock();
  std::thread(_run, std::ref(_mtx), _instance, std::ref(_name_to_obj),
      std::ref(_should_exit), std::ref(address), port).detach();
}
void NOSAgent::exit() { // assumes Initialize was called, don't call more than once
  _mtx.lock();
  _should_exit = true;
  _mtx.unlock();
}

void NOSAgent::Export(const std::string name, NetObj* netObj) {
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
void NOSAgent::print_exported() {
  _mtx.lock();
  for (auto it = _name_to_obj.begin(); it != _name_to_obj.end(); ++it) {
    std::cout << typeid(*(it->second->agentObj)).name() << " " << it->first
      << std::endl;
  }
  _mtx.unlock();
}
// mark obj with name as deleted by server (precondition for GC)
void NOSAgent::mark_obj_deleted(std::string name) {
  _mtx.lock();
  if (_name_to_obj.find(name) != _name_to_obj.end()) {
    _name_to_obj[name]->server_deleted = true;
    std::cout << name << " marked as deleted by server" << std::endl;
  }
  _mtx.unlock();
}

RPCRequest NOSAgent::rpc_receive(TCPStream* stream) {
  std::string buffer;
  receive_packet(stream, buffer);

  RPCRequest request = RPCRequest::load_packet(buffer);
  if (_debugMode) {
      std::cerr << "Request:" << request.to_str() << "\n";
  }
  return request;
}
