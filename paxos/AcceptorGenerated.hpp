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

    // Methods without return values

    // Getters and setters for public fields

    getMinAcceptNum,
    setMinAcceptNum,

    getAccepted,
    setAccepted,

};

class AcceptorServer : public Acceptor {
    friend class AcceptorAgent;

  public:

    AcceptorServer() : _base(new Acceptor()) {}


    ~AcceptorServer() {
        _agent->mark_obj_deleted(_name);
    }




    uint32_t getMinAcceptNum() {
        return _base->_min_accept_num;
    }

    void setMinAcceptNum(uint32_t &value) {
        _base->_min_accept_num = value;
    }

    std::map<uint32_t, std::string> getAccepted() {
        return _base->_accepted;
    }

    void setAccepted(std::map<uint32_t, std::string> &value) {
        _base->_accepted = value;
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



            case AcceptorMethodID::getMinAcceptNum: {
                std::cout << "dispatch: getMinAcceptNum" << std::endl;
                //auto args = Serializer::unpack<std::tuple<>>(request.Body);
                //auto result = TupleFunctional::apply_nonstatic_fn(&Acceptor::getMinAcceptNum, _base, args);
                auto result = _base->_min_accept_num;
                response.Body = Serializer::pack<decltype(result)>(result);
                response.Code = ServerCode::OK;
                break;
            }

            case AcceptorMethodID::setMinAcceptNum: {
                std::cout << "dispatch: setMinAcceptNum" << std::endl;
                auto args = Serializer::unpack<std::tuple<uint32_t>>(request.Body);
                //TupleFunctional::apply_nonstatic_fn(&Acceptor::setMinAcceptNum, _base, args);
                _base->_min_accept_num = std::get<0>(args);
                response.Code = ServerCode::OK;
                break;
            }

            case AcceptorMethodID::getAccepted: {
                std::cout << "dispatch: getAccepted" << std::endl;
                //auto args = Serializer::unpack<std::tuple<>>(request.Body);
                //auto result = TupleFunctional::apply_nonstatic_fn(&Acceptor::getAccepted, _base, args);
                auto result = _base->_accepted;
                response.Body = Serializer::pack<decltype(result)>(result);
                response.Code = ServerCode::OK;
                break;
            }

            case AcceptorMethodID::setAccepted: {
                std::cout << "dispatch: setAccepted" << std::endl;
                auto args = Serializer::unpack<std::tuple<std::map<uint32_t, std::string>>>(request.Body);
                //TupleFunctional::apply_nonstatic_fn(&Acceptor::setAccepted, _base, args);
                _base->_accepted = std::get<0>(args);
                response.Code = ServerCode::OK;
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



    uint32_t getMinAcceptNum() {
        auto args = std::make_tuple();
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(AcceptorMethodID::getMinAcceptNum),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
        return Serializer::unpack<uint32_t>(response.Body);
    }

    void setMinAcceptNum(uint32_t &value) {
        auto args = std::tuple< uint32_t > { value };
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(AcceptorMethodID::setMinAcceptNum),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
    }

    std::map<uint32_t, std::string> getAccepted() {
        auto args = std::make_tuple();
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(AcceptorMethodID::getAccepted),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
        return Serializer::unpack<std::map<uint32_t, std::string>>(response.Body);
    }

    void setAccepted(std::map<uint32_t, std::string> &value) {
        auto args = std::tuple< std::map<uint32_t, std::string> > { value };
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(AcceptorMethodID::setAccepted),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
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
