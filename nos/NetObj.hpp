#pragma once

#include <string>

class NOSClient;
class RPCRequest;
class RPCResponse;
class NetObj {
 public:
  virtual ~NetObj() {}
};

class ClientObj {
 public:
  ClientObj(std::string name, NOSClient* client, std::string address,
            uint32_t port)
    : _name(name), _client(client), _address(address), _port(port) {}
  virtual ~ClientObj() {}

  std::string _name;
  NOSClient* _client;
  std::string _address;
  uint32_t _port;
};

class AgentObj {
 public:
  virtual ~AgentObj() {}
  virtual void dispatch(RPCRequest& request, RPCResponse& response) = 0;
};
