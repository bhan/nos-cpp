// AUTO GENERATED
#ifndef _BASEB_GENERATED_HPP
#define _BASEB_GENERATED_HPP

#include <iostream>
#include <stdexcept>

#include "BaseB.hpp"
#include "../nos/NetObj.hpp"
#include "../nos/NOSAgent.hpp"
#include "../nos/NOSClient.hpp"
#include "../nos/RPCRequest.hpp"
#include "../nos/RPCResponse.hpp"
#include "../nos/Serializer.hpp"
#include "../nos/TupleFunctional.hpp"

enum class BaseBMethodID : uint32_t {
    // Methods with return values

    increment,

    decrement,

    bork_method,

    // Methods without return values

    bark_method,

    void_method,

    // Getters and setters for public fields

    getBar,
    setBar,

    getBaz,
    setBaz,

};

class BaseBServer : public BaseB {
    friend class BaseBAgent;

  public:

    BaseBServer() : _base(new BaseB()) {}

    BaseBServer(int32_t num) : _base(new BaseB(num)) {}


    ~BaseBServer() {
        _agent->mark_obj_deleted(_name);
    }


    int32_t increment(int32_t a) {
        return _base->increment(a);
    }

    int32_t decrement(int a) {
        return _base->decrement(a);
    }

    bool bork_method(uint32_t a, std::string b) {
        return _base->bork_method(a, b);
    }


    void bark_method(std::vector<double> a) {
        _base->bark_method(a);
    }

    void void_method() {
        _base->void_method();
    }


    std::vector<std::string> getBar() {
        return _base->bar;
    }

    void setBar(std::vector<std::string> &value) {
        _base->bar = value;
    }

    double getBaz() {
        return _base->baz;
    }

    void setBaz(double &value) {
        _base->baz = value;
    }

  private:
    BaseB* _base;
    NOSAgent* _agent;
    std::string _name;
};

class BaseBAgent : public AgentObj {
  public:
    BaseBAgent(NetObj* obj, std::string name, NOSAgent* agent) {
        BaseBServer* server = dynamic_cast<BaseBServer*>(obj);
        _base = server->_base;
        server->_name = name;
        server->_agent = agent;
    }
    ~BaseBAgent() {
        delete _base;
    }

    void dispatch(RPCRequest& request, RPCResponse& response) {
        switch (static_cast<BaseBMethodID>(request.MethodID)) {

            case BaseBMethodID::increment: {
                std::cout << "dispatch: increment" << std::endl;
                auto args = Serializer::unpack< std::tuple< int32_t > >(request.Body);
                auto result = TupleFunctional::apply_nonstatic_fn(&BaseB::increment, _base, args);
                response.Code = ServerCode::OK;
                response.Body = Serializer::pack<decltype(result)>(result);
                break;
            }

            case BaseBMethodID::decrement: {
                std::cout << "dispatch: decrement" << std::endl;
                auto args = Serializer::unpack< std::tuple< int > >(request.Body);
                auto result = TupleFunctional::apply_nonstatic_fn(&BaseB::decrement, _base, args);
                response.Code = ServerCode::OK;
                response.Body = Serializer::pack<decltype(result)>(result);
                break;
            }

            case BaseBMethodID::bork_method: {
                std::cout << "dispatch: bork_method" << std::endl;
                auto args = Serializer::unpack< std::tuple< uint32_t, std::string > >(request.Body);
                auto result = TupleFunctional::apply_nonstatic_fn(&BaseB::bork_method, _base, args);
                response.Code = ServerCode::OK;
                response.Body = Serializer::pack<decltype(result)>(result);
                break;
            }


            case BaseBMethodID::bark_method: {
                std::cout << "dispatch: bark_method" << std::endl;
                auto args = Serializer::unpack< std::tuple< std::vector<double> > >(request.Body);
                TupleFunctional::apply_nonstatic_fn(&BaseB::bark_method, _base, args);
                response.Code = ServerCode::OK;
                break;
            }

            case BaseBMethodID::void_method: {
                std::cout << "dispatch: void_method" << std::endl;
                auto args = Serializer::unpack< std::tuple<  > >(request.Body);
                TupleFunctional::apply_nonstatic_fn(&BaseB::void_method, _base, args);
                response.Code = ServerCode::OK;
                break;
            }


            case BaseBMethodID::getBar: {
                std::cout << "dispatch: getBar" << std::endl;
                //auto args = Serializer::unpack<std::tuple<>>(request.Body);
                //auto result = TupleFunctional::apply_nonstatic_fn(&BaseB::getBar, _base, args);
                auto result = _base->bar;
                response.Body = Serializer::pack<decltype(result)>(result);
                response.Code = ServerCode::OK;
                break;
            }

            case BaseBMethodID::setBar: {
                std::cout << "dispatch: setBar" << std::endl;
                auto args = Serializer::unpack<std::tuple<std::vector<std::string>>>(request.Body);
                //TupleFunctional::apply_nonstatic_fn(&BaseB::setBar, _base, args);
                _base->bar = std::get<0>(args);
                response.Code = ServerCode::OK;
                break;
            }

            case BaseBMethodID::getBaz: {
                std::cout << "dispatch: getBaz" << std::endl;
                //auto args = Serializer::unpack<std::tuple<>>(request.Body);
                //auto result = TupleFunctional::apply_nonstatic_fn(&BaseB::getBaz, _base, args);
                auto result = _base->baz;
                response.Body = Serializer::pack<decltype(result)>(result);
                response.Code = ServerCode::OK;
                break;
            }

            case BaseBMethodID::setBaz: {
                std::cout << "dispatch: setBaz" << std::endl;
                auto args = Serializer::unpack<std::tuple<double>>(request.Body);
                //TupleFunctional::apply_nonstatic_fn(&BaseB::setBaz, _base, args);
                _base->baz = std::get<0>(args);
                response.Code = ServerCode::OK;
                break;
            }

            default: {
                std::stringstream err;
                err << "dispatch: unsupported dispatch code " << request.MethodID << " for network object type \'BaseB\'\n";
                response.Code = ServerCode::FAIL;
                response.Body = err.str();
                std::cerr << response.Body;
                break;
            }
        }
    }
  private:
    BaseB* _base;
};

class BaseBClient : public ClientObj {
  public:
    BaseBClient(std::string name, NOSClient* client, std::string address, uint32_t port)
                        : ClientObj(name, client, address, port) {}

    ~BaseBClient() {
        _client->mark_obj_deleted(_name);
    }

    int32_t increment(int32_t a) {
        auto args = std::make_tuple(a);
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(BaseBMethodID::increment),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
        return Serializer::unpack<int32_t>(response.Body);
    }

    int32_t decrement(int a) {
        auto args = std::make_tuple(a);
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(BaseBMethodID::decrement),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
        return Serializer::unpack<int32_t>(response.Body);
    }

    bool bork_method(uint32_t a, std::string b) {
        auto args = std::make_tuple(a, b);
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(BaseBMethodID::bork_method),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
        return Serializer::unpack<bool>(response.Body);
    }


    void bark_method(std::vector<double> a) {
        auto args = std::make_tuple(a);
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(BaseBMethodID::bark_method),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
    }

    void void_method() {
        auto args = std::make_tuple();
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(BaseBMethodID::void_method),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
    }


    std::vector<std::string> getBar() {
        auto args = std::make_tuple();
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(BaseBMethodID::getBar),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
        return Serializer::unpack<std::vector<std::string>>(response.Body);
    }

    void setBar(std::vector<std::string> &value) {
        auto args = std::tuple< std::vector<std::string> > { value };
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(BaseBMethodID::setBar),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
    }

    double getBaz() {
        auto args = std::make_tuple();
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(BaseBMethodID::getBaz),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
        return Serializer::unpack<double>(response.Body);
    }

    void setBaz(double &value) {
        auto args = std::tuple< double > { value };
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(BaseBMethodID::setBaz),
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

#endif /* _BASEB_GENERATED_HPP */
