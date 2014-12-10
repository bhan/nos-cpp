// AUTO GENERATED
#ifndef _ACCEPTOR_GENERATED_HPP
#define _ACCEPTOR_GENERATED_HPP

#include <iostream>
#include <stdexcept>

#include "Acceptor.hpp"
#include "../nos/NetObj.hpp"
#include "../nos/NOSAgent.hpp"
#include "../nos/NOSClient.hpp"
#include "../nos/RPCRequest.hpp"
#include "../nos/RPCResponse.hpp"
#include "../nos/Serializer.hpp"
#include "../nos/TupleFunctional.hpp"

enum class AcceptorMethodID : uint32_t {
    // Methods with return values

    prepare,

    accept,

    // Methods without return values

    // Getters and setters for public fields

};

class AcceptorServer : public Acceptor {
    friend class AcceptorAgent;

  public:

    AcceptorServer() : _base(new Acceptor()) {}


    ~AcceptorServer() {
        _agent->mark_obj_deleted(_name);
    }


    std::tuple<uint32_t, std::string> prepare(uint32_t num) {
        return _base->prepare(num);
    }

    bool accept(std::tuple<uint32_t, std::string> accept_request) {
        return _base->accept(accept_request);
    }



  private:
    Acceptor* _base;
    NOSAgent* _agent;
    std::string _name;
};

class AcceptorAgent : public AgentObj {
  public:
    AcceptorAgent(NetObj* obj, std::string name, NOSAgent* agent) {
        AcceptorServer* server = dynamic_cast<AcceptorServer*>(obj);
        _base = server->_base;
        server->_name = name;
        server->_agent = agent;
    }
    ~AcceptorAgent() {
        delete _base;
    }

    void dispatch(RPCRequest& request, RPCResponse& response) {
        switch (static_cast<AcceptorMethodID>(request.MethodID)) {

            case AcceptorMethodID::prepare: {
                std::cout << "dispatch: prepare" << std::endl;
                auto args = Serializer::unpack< std::tuple< uint32_t > >(request.Body);
                auto result = TupleFunctional::apply_nonstatic_fn(&Acceptor::prepare, _base, args);
                response.Code = ServerCode::OK;
                response.Body = Serializer::pack<decltype(result)>(result);
                break;
            }

            case AcceptorMethodID::accept: {
                std::cout << "dispatch: accept" << std::endl;
                auto args = Serializer::unpack< std::tuple< std::tuple<uint32_t, std::string> > >(request.Body);
                auto result = TupleFunctional::apply_nonstatic_fn(&Acceptor::accept, _base, args);
                response.Code = ServerCode::OK;
                response.Body = Serializer::pack<decltype(result)>(result);
                break;
            }



            default: {
                std::stringstream err;
                err << "dispatch: unsupported dispatch code " << request.MethodID << " for network object type \'Acceptor\'\n";
                response.Code = ServerCode::FAIL;
                response.Body = err.str();
                std::cerr << response.Body;
                break;
            }
        }
    }
  private:
    Acceptor* _base;
};

class AcceptorClient : public ClientObj {
  public:
    AcceptorClient(std::string name, NOSClient* client, std::string address, uint32_t port)
                        : ClientObj(name, client, address, port) {}

    ~AcceptorClient() {
        _client->mark_obj_deleted(_name);
    }

    std::tuple<uint32_t, std::string> prepare(uint32_t num) {
        auto args = std::make_tuple(num);
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(AcceptorMethodID::prepare),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
        return Serializer::unpack<std::tuple<uint32_t, std::string>>(response.Body);
    }

    bool accept(std::tuple<uint32_t, std::string> accept_request) {
        auto args = std::make_tuple(accept_request);
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(AcceptorMethodID::accept),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
        return Serializer::unpack<bool>(response.Body);
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

#endif /* _ACCEPTOR_GENERATED_HPP */
