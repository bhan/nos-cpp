// AUTO GENERATED
#ifndef _BASEA_GENERATED_HPP
#define _BASEA_GENERATED_HPP

#include <iostream>
#include <stdexcept>

#include "BaseA.hpp"
#include "../nos/NetObj.hpp"
#include "../nos/NOSAgent.hpp"
#include "../nos/NOSClient.hpp"
#include "../nos/RPCRequest.hpp"
#include "../nos/RPCResponse.hpp"
#include "../nos/Serializer.hpp"
#include "../nos/TupleFunctional.hpp"

enum class BaseAMethodID : uint32_t {
    // Methods with return values

    decrement,

    increment,

    // Methods without return values

    // Getters and setters for public fields

};

class BaseAServer : public BaseA {
    friend class BaseAAgent;

  public:

    BaseAServer() : _base(new BaseA()) {}

    BaseAServer(int32_t num) : _base(new BaseA(num)) {}


    ~BaseAServer() {
        _agent->mark_obj_deleted(_name);
    }


    int32_t decrement() {
        return _base->decrement();
    }

    int32_t increment() {
        return _base->increment();
    }



  private:
    BaseA* _base;
    NOSAgent* _agent;
    std::string _name;
};

class BaseAAgent : public AgentObj {
  public:
    BaseAAgent(NetObj* obj, std::string name, NOSAgent* agent) {
        BaseAServer* server = dynamic_cast<BaseAServer*>(obj);
        _base = server->_base;
        server->_name = name;
        server->_agent = agent;
    }
    ~BaseAAgent() {
        delete _base;
    }

    void dispatch(RPCRequest& request, RPCResponse& response) {
        switch (static_cast<BaseAMethodID>(request.MethodID)) {

            case BaseAMethodID::decrement: {
                std::cout << "dispatch: decrement" << std::endl;
                auto args = Serializer::unpack< std::tuple<  > >(request.Body);
                auto result = TupleFunctional::apply_nonstatic_fn(&BaseA::decrement, _base, args);
                response.Code = ServerCode::OK;
                response.Body = Serializer::pack<decltype(result)>(result);
                break;
            }

            case BaseAMethodID::increment: {
                std::cout << "dispatch: increment" << std::endl;
                auto args = Serializer::unpack< std::tuple<  > >(request.Body);
                auto result = TupleFunctional::apply_nonstatic_fn(&BaseA::increment, _base, args);
                response.Code = ServerCode::OK;
                response.Body = Serializer::pack<decltype(result)>(result);
                break;
            }



            default: {
                std::stringstream err;
                err << "dispatch: unsupported dispatch code " << request.MethodID << " for network object type \'BaseA\'\n";
                response.Code = ServerCode::FAIL;
                response.Body = err.str();
                std::cerr << response.Body;
                break;
            }
        }
    }
  private:
    BaseA* _base;
};

class BaseAClient : public ClientObj {
  public:
    BaseAClient(std::string name, NOSClient* client, std::string address, uint32_t port)
                        : ClientObj(name, client, address, port) {}

    ~BaseAClient() {
        _client->mark_obj_deleted(_name);
    }

    int32_t decrement() {
        auto args = std::make_tuple();
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(BaseAMethodID::decrement),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
        return Serializer::unpack<int32_t>(response.Body);
    }

    int32_t increment() {
        auto args = std::make_tuple();
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(BaseAMethodID::increment),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
        return Serializer::unpack<int32_t>(response.Body);
    }



  private:
    void handle_rpc_exceptions(RPCResponse &response) {
        if (response.Code == ServerCode::FAIL) {
            throw std::runtime_error(std::string("Method Invocation Failed -> Server Response:\n") + response.Body + "\n");
        } else if (response.Code != ServerCode::OK) {
            throw std::runtime_error("Network Error");
        }
    }
};

#endif /* _BASEA_GENERATED_HPP */
