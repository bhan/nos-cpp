#pragma once

#include <mutex>
#include <string>
#include <unordered_map>

#include "../tcpsockets/tcpconnector.h"
#include "NetObj.hpp"
#include "NOSCommon.hpp"
#include "RPCRequest.hpp"
#include "RPCResponse.hpp"
#include "NOSClientTypeUtil.hpp"

class NOSClient : protected NOSCommon, protected TCPConnector {
public:
  static NOSClient* Instance(uint32_t timeout=10, bool debugMode=true) {
    if (_instance == nullptr) { _instance = new NOSClient(timeout, debugMode); }
    return _instance;
  }
  void mark_obj_deleted(std::string objectID);
  void initialize(uint renew_seconds);
  ClientObj* Import(std::string name, std::string& address, uint32_t port);
  void exit();
  RPCResponse rpc_send(const RPCRequest &request,
                       std::string& address, uint32_t port);
private:
  bool send(const std::string &buffer, std::string &responseBuffer,
            std::string& address, uint32_t port);

  NOSClient(uint32_t timeout, bool debugMode)
    : TCPConnector(), _timeoutSeconds(timeout), _debugMode(debugMode) {}
  NOSClient(const NOSClient&) {}
  NOSClient& operator=(const NOSClient&) { return *_instance;}

  static NOSClient* _instance;
  NOSClientTypeUtil _type_util;
  uint32_t _timeoutSeconds;
  bool _debugMode;
  volatile bool _should_exit; // tell the detached thread to stop looping

  std::mutex _mtx;
  std::unordered_map<std::string, ClientObj*> _ObjectID_to_ClientObj;
};
