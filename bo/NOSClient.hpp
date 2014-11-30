#pragma once
#include "../tcpsockets/tcpconnector.h"
#include "NetObj.hpp"
#include "NOSCommon.hpp"
#include "RPCRequest.hpp"
#include "RPCResponse.hpp"
#include "TypeUtil.hpp"

class NOSClient : protected NOSCommon, protected TCPConnector {
public:
  static NOSClient* Instance(uint32_t timeout=10, bool debugMode=true) {
    if (_instance == nullptr) { _instance = new NOSClient(timeout, debugMode); }
    return _instance;
  }
  ClientObj* Import(std::string name, std::string& address, uint32_t port);
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
  TypeUtil _type_util;
  uint32_t _timeoutSeconds;
  bool _debugMode;
};
