// AUTO GENERATED
#ifndef _BASE_B_GENERATED_H
#define _BASE_B_GENERATED_H

#include <arpa/inet.h>
#include <iostream>
#include <stdexcept>

#include "NOSAgent.hpp"
#include "NOSClient.hpp"
#include "BaseB.hpp"
#include "NetObj.hpp"
#include "Codes.hpp"
#include "RPCRequest.hpp"
#include "RPCResponse.hpp"
#include "Serializer.hpp"
#include "TupleFunctional.hpp"
#include "../tcpsockets/tcpconnector.h"
#include "../tcpsockets/tcpstream.h"

enum class BaseBMethodID : uint32_t {
  invalid,
  decrement,
  increment,
};

class BaseBServer : public BaseB {
  friend class BaseBAgent;
 public:
  BaseBServer(int32_t num) : _base(new BaseB(num)) {}
  ~BaseBServer() {
    _agent->mark_obj_deleted(_name);
  }
  int32_t decrement(int32_t a) {
    return _base->decrement(a);
  }
  int32_t increment(int32_t a) {
    return _base->increment(a);
  }
 private:
  BaseB* _base;
  NOSAgent* _agent;
  std::string _name;
};

class BaseBAgent : public AgentObj {
 public:
  BaseBAgent(NetObj* obj, std::string name, NOSAgent* agent) {
    BaseBServer* baseBServer = dynamic_cast<BaseBServer*>(obj);
    _base = baseBServer->_base;
    baseBServer->_name = name;
    baseBServer->_agent = agent;
  }
  void dispatch(RPCRequest& request, RPCResponse& response) {
    switch (static_cast<BaseBMethodID>(request.MethodID)) {
      case BaseBMethodID::decrement: {
        std::cout << "dispatch: decrement" << std::endl;
        auto args = Serializer::unpack<std::tuple<int32_t>>(request.Body);
        auto result = TupleFunctional::apply_nonstatic_fn(&BaseB::decrement, _base, args);
        response.Code = ServerCode::OK;
        response.Body = Serializer::pack<decltype(result)>(result);
        break;
      }
      case BaseBMethodID::increment: {
        std::cout << "dispatch: increment" << std::endl;
        auto args = Serializer::unpack<std::tuple<int32_t>>(request.Body);
        auto result = TupleFunctional::apply_nonstatic_fn(&BaseB::increment, _base, args);
        response.Code = ServerCode::OK;
        response.Body = Serializer::pack<decltype(result)>(result);
        break;
      }
      default: {
        std::cout << "dispatch: unsupported method" << std::endl;
        response.Code = ServerCode::FAIL;
        break;
      }
    }
  }
 private:
  BaseB* _base;
};

class BaseBClient : public ClientObj {
 public:
  BaseBClient(std::string name, NOSClient* client,
              std::string address, uint32_t port)
    : ClientObj(name, client, address, port) {}
  ~BaseBClient() {
    _client->mark_obj_deleted(_name);
  }
  int32_t decrement(int32_t a) {
    auto args = std::make_tuple(a);
    RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                       static_cast<uint32_t>(BaseBMethodID::decrement),
                       Serializer::pack<decltype(args)>(args));
    RPCResponse response = _client->rpc_send(request, _address, _port);
    if (response.Code != ServerCode::OK)
      throw std::runtime_error("network error");
    auto return_tuple = Serializer::unpack<std::tuple<int32_t>>(response.Body);
    return std::get<0>(return_tuple);
  }

  int32_t increment(int32_t a) {
    auto args = std::make_tuple(a);
    RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                       static_cast<uint32_t>(BaseBMethodID::increment),
                       Serializer::pack<decltype(args)>(args));
    RPCResponse response = _client->rpc_send(request, _address, _port);
    if (response.Code != ServerCode::OK)
      throw std::runtime_error("network error");
    auto return_tuple = Serializer::unpack<std::tuple<int32_t>>(response.Body);
    return std::get<0>(return_tuple);
  }
};

#endif /* _BASE_B_GENERATED_H */
