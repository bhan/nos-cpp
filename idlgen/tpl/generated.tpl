// AUTO GENERATED
#ifndef _{{CLASS_UPPERCASE}}_GENERATED_H
#define _{{CLASS_UPPERCASE}}_GENERATED_H

#include <arpa/inet.h>
#include <iostream>
#include <stdexcept>

#include "{{CLASS_NAME}}.hpp"
#include "ClientObj.hpp"
#include "Codes.hpp"
#include "Serialize.hpp"
#include "../tcpsockets/tcpconnector.h"
#include "../tcpsockets/tcpstream.h"

class {{CLASS_NAME}}Server : public {{CLASS_NAME}} {
  friend class {{CLASS_NAME}}Agent;

 public:

{{#CTORS}}
  {{CLASS_NAME}}Server({{CTOR_ARGS_WITH_TYPES}}) : _base(new {{CLASS_NAME}}({{CTOR_ARGS}})) {}
{{/CTORS}}

  ~{{CLASS_NAME}}Server() {
    _agent->mark_obj_deleted(_name);
  }

{{#METHOD_IMPLS}}
  {{METHOD_RET_TYPE}} {{METHOD_NAME}}({{METHOD_ARGS_WITH_TYPES}}) {
    return _base->{{METHOD_NAME}}({{METHOD_ARGS}});
  }
{{/METHOD_IMPLS}}

 private:
  {{CLASS_NAME}}* _base;
  NOSAgent* _agent;
  std::string _name;
};

class {{CLASS_NAME}}Agent : public AgentObj {
 public:

  {{CLASS_NAME}}Agent(NetObj* obj, std::string name, NOSAgent* agent) {
    {{CLASS_NAME}}Server* server = dynamic_cast<{{CLASS_NAME}}Server*>(obj);
    _base = server->_base;
    server->_name = name;
    server->_agent = agent;
  }

  void dispatch(RPCRequest& request, RPCResponse& response) {
    switch (static_cast<{{CLASS_NAME}}MethodID>(request.MethodID)) {
{{#METHOD_IMPLS}}
      case {{CLASS_NAME}}MethodID::{{METHOD_NAME}}: {
        std::cout << "dispatch: {{METHOD_NAME}}" << std::endl;
        auto args = Serializer::unpack<std::tuple<{{METHOD_ARGS_TYPES}}>>(request.Body);
        auto result = TupleFunctions::apply_nonstatic_fn(&{{CLASS_NAME}}::{{METHOD_NAME}}, _base, args);
        response.Code = ServerCode::OK;
        response.Body = Serializer::pack<decltype(result)>(result);
        break;
      }
{{/METHOD_IMPLS}}
      default: {
        std::cout << "dispatch: unsupported" << std::endl;
        response.Code = ServerCode::FAIL;
        break;
      }
    }
  }
 private:
  {{CLASS_NAME}}* _base;
};

class {{CLASS_NAME}}Client : public ClientObj {
 public:
  {{CLASS_NAME}}Client(std::string name, NOSClient* client, std::string address, uint32_t port)
        : ClientObj(name, client, address, port) {}
  ~{{CLASS_NAME}}Client() {
    _client->mark_obj_deleted(_name);
  }

{{#METHOD_IMPLS}}
  {{METHOD_RET_TYPE}} {{METHOD_NAME}}({{METHOD_ARGS_WITH_TYPES}}) {
    auto args = std::make_tuple({{METHOD_ARGS}});
    RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                       static_cast<uint32_t>({{CLASS_NAME}}MethodID::{{METHOD_NAME}}),
                       Serializer::pack<decltype(args)>(args));
    RPCResponse response = _client->rpc_send(request, _address, _port);
    if (response.Code != ServerCode::OK)
      throw std::runtime_error("network error");
    auto return_tuple = Serializer::unpack<std::tuple<{{METHOD_RET_TYPE}}>>(response.Body);
    return std::get<0>(return_tuple);
  }
{{/METHOD_IMPLS}}

{{#PUBLIC_FIELDS}}
  {{FIELD_TYPE}} get{{FIELD_NAME_CAMEL_CASE}}() {
    // TODO fill in code
  }

  void set{{FIELD_NAME_CAMEL_CASE}}({{FIELD_TYPE}} value) {
    // TODO fill in code
  }
{{/PUBLIC_FIELDS}}
};

#endif /* _BASE_B_GENERATED_H */
