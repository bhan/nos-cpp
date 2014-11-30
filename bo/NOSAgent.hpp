#pragma once

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

class NOSAgent : public NOSCommon {
 public:
  static NOSAgent* Instance(bool _debugMode = false) {
    if (_instance == nullptr) { _instance = new NOSAgent(_debugMode); }
    return _instance;
  }
  void initialize(std::string address, uint32_t port, uint expire_seconds);
  void exit();
  void Export(const std::string name, NetObj* netObj);
  void mark_obj_deleted(std::string name);
  void print_exported();
  RPCRequest rpc_receive(TCPStream* stream);
 private:
  NOSAgent(bool debugMode) : _debugMode(debugMode) {}
  NOSAgent(const NOSAgent&) {}
  NOSAgent& operator=(const NOSAgent&) { return *_instance;}

  static NOSAgent* _instance;
  std::unordered_map<std::string, ObjEntry*> _name_to_obj;
  std::mutex _mtx;
  volatile bool _should_exit; // tell the detached thread to stop looping
  TypeUtil _type_util;
  bool _debugMode = false;
};
