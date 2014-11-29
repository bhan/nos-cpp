#ifndef _AGENT_H
#define _AGENT_H

#include <string>
#include <unordered_map>

#include "NetObj.hpp"
#include "TypeUtil.hpp"
#include "NOSCommon.hpp"
#include "RPCRequest.hpp"
#include "../tcpsockets/tcpacceptor.h"
#include "../tcpsockets/tcpconnector.h"
#include "../tcpsockets/tcpstream.h"

class ObjEntry;

class Agent : public NOSCommon {
 public:
  static Agent* Instance() {
    if (_instance == nullptr) { _instance = new Agent(); }
    return _instance;
  }
  void Initialize(int port, std::string ip_addr);
  void Exit();
  ClientObj* Import(std::string name, int port, std::string ip_addr);
  void Export(const std::string name, NetObj* netObj);
  void PrintExported();
  void mark_obj_deleted(std::string name);
 private:
  Agent() {}
  Agent(const Agent&) {}
  Agent& operator=(const Agent&) { return *_instance;}
  RPCRequest rpc_receive(TCPStream* stream);
  static Agent* _instance;
  std::unordered_map<std::string, ObjEntry*> _name_to_obj;
  std::mutex _mtx;
  volatile bool _should_exit; // tell the detached thread to stop looping
  TypeUtil _type_util;
  bool _debugMode = false;
};

#endif /*  _AGENT_H */
