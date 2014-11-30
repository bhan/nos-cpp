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
#include "RPCResponse.hpp"
#include "Serializer.hpp"

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

static void _garbage_collect(std::mutex& mtx,
    std::unordered_map<std::string, ObjEntry*>& name_to_obj,
    uint expire_seconds, volatile bool& should_exit) {
  mtx.lock();
  while (!should_exit) {
    mtx.unlock();
    time_t cur_time = time(NULL);
    std::cout << "garbage collection at " << cur_time << std::endl;

    mtx.lock();
    for (auto it = name_to_obj.begin(); it != name_to_obj.end(); ) {
      std::cout << it->first << " server_deleted? " << it->second->server_deleted << std::endl;
      if (!it->second->server_deleted) {
        ++it;
        continue;
      }
      double diff_time = difftime(cur_time, it->second->renewed_time);
      std::cout << "difftime: " << diff_time << std::endl;
      if (diff_time <= expire_seconds) {
        ++it;
        continue;
      }
      auto erased = it->first;
      auto old_it = it;
      ++it;
      name_to_obj.erase(old_it);
      std::cout << "erased " << erased << " and its entry" << std::endl;
    }
    mtx.unlock();

    std::this_thread::sleep_for(std::chrono::seconds(2 * expire_seconds));
    mtx.lock();
  }
}

static void _run(std::mutex& mtx, NOSAgent* agent,
    std::unordered_map<std::string, ObjEntry*>& name_to_obj,
    volatile bool& should_exit, std::string& address, uint32_t port) {
  mtx.lock();
  TCPAcceptor* acceptor = new TCPAcceptor(port, address.c_str());
  if (acceptor->start() != 0) {
    std::cout << "NOSAgent failed to start TCP acceptor" << std::endl;
    ::exit(1);
  }

  std::cout << "NOSAgent started at " << address << ":" << port << std::endl;
  while (!should_exit) {
    mtx.unlock();

    RPCRequest request;
    RPCResponse response;
    TCPStream* stream = acceptor->accept();
    if (stream == NULL) { goto err_exit; }

    request = agent->rpc_receive(stream);
    std::cout << request.to_str() << std::endl;
    switch (static_cast<RequestType>(request.Type)) {
      case RequestType::get_type: {
        std::cout << "get_type" << std::endl;
        mtx.lock();
        auto it = name_to_obj.find(request.ObjectID);
        if (it == name_to_obj.end()) {
          response.Code = ServerCode::OBJECT_NOT_FOUND;
        } else { // found NetObj for name
          std::string type = typeid(*(it->second->agentObj)).name();
          response.Body = type;
          response.Code = ServerCode::OK;
        }
        mtx.unlock();
        break;
      }
      case RequestType::invoke: {
        std::cout << "invoke requested for " << request.ObjectID << std::endl;
        mtx.lock();
        auto it = name_to_obj.find(request.ObjectID);
        if (it == name_to_obj.end()) {
          response.Code = ServerCode::OBJECT_NOT_FOUND;
          goto send_response;
        }
        (it->second->agentObj)->dispatch(request, response);
        mtx.unlock();
        break;
      }
      case RequestType::renew_lease: {
        std::cout << "renew_lease requested for " << request.ObjectID << std::endl;
        mtx.lock();
        auto it = name_to_obj.find(request.ObjectID);
        if (it == name_to_obj.end()) {
          response.Code = ServerCode::OBJECT_NOT_FOUND;
          goto send_response;
        }
        it->second->renewed_time = time(NULL);
        std::cout << "renewed_time " << it->second->renewed_time << " for " << request.ObjectID << std::endl;
        response.Code = ServerCode::OK;
        mtx.unlock();
        break;
      }
      default: {
        std::cout << "no valid RequestType received" << std::endl;
        break;
      }
    }
send_response:
    agent->send_packet(stream, response.packet());
cleanup_stream: delete stream;
err_exit:
    mtx.lock();
  }
  // TODO thread cleanup
}
void NOSAgent::initialize(std::string address, uint32_t port,
                          uint expire_seconds) { // don't call more than once
  _should_exit = false;
  std::thread(_run, std::ref(_mtx), _instance, std::ref(_name_to_obj),
      std::ref(_should_exit), std::ref(address), port).detach();
  std::thread(_garbage_collect, std::ref(_mtx), std::ref(_name_to_obj),
              expire_seconds, std::ref(_should_exit)).detach();
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
