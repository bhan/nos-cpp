// AUTO GENERATED
#ifndef _KVSTORE_GENERATED_HPP
#define _KVSTORE_GENERATED_HPP

#include <iostream>
#include <stdexcept>

#include "../netobj_classes/KVStore.hpp"
#include "../nos/NetObj.hpp"
#include "../nos/NOSAgent.hpp"
#include "../nos/NOSClient.hpp"
#include "../nos/RPCRequest.hpp"
#include "../nos/RPCResponse.hpp"
#include "../nos/Serializer.hpp"
#include "../nos/TupleFunctional.hpp"

enum class KVStoreMethodID : uint32_t {
    // Methods with return values

    put,

    get,

    // Methods without return values

    // Getters and setters for public fields

};

class KVStoreServer : public KVStore {
    friend class KVStoreAgent;

  public:

    KVStoreServer() : _base(new KVStore()) {}


    ~KVStoreServer() {
        _agent->mark_obj_deleted(_name);
    }


    bool put(std::string key, std::string value) {
        return _base->put(key, value);
    }

    std::string get(std::string key) {
        return _base->get(key);
    }



  private:
    KVStore* _base;
    NOSAgent* _agent;
    std::string _name;
};

class KVStoreAgent : public AgentObj {
  public:
    KVStoreAgent(NetObj* obj, std::string name, NOSAgent* agent) {
        KVStoreServer* server = dynamic_cast<KVStoreServer*>(obj);
        _base = server->_base;
        server->_name = name;
        server->_agent = agent;
    }

    void dispatch(RPCRequest& request, RPCResponse& response) {
        switch (static_cast<KVStoreMethodID>(request.MethodID)) {

            case KVStoreMethodID::put: {
                std::cout << "dispatch: put" << std::endl;
                auto args = Serializer::unpack< std::tuple< std::string, std::string > >(request.Body);
                auto result = TupleFunctional::apply_nonstatic_fn(&KVStore::put, _base, args);
                response.Code = ServerCode::OK;
                response.Body = Serializer::pack<decltype(result)>(result);
                break;
            }

            case KVStoreMethodID::get: {
                std::cout << "dispatch: get" << std::endl;
                auto args = Serializer::unpack< std::tuple< std::string > >(request.Body);
                auto result = TupleFunctional::apply_nonstatic_fn(&KVStore::get, _base, args);
                response.Code = ServerCode::OK;
                response.Body = Serializer::pack<decltype(result)>(result);
                break;
            }



            default: {
                std::stringstream err;
                err << "dispatch: unsupported dispatch code " << request.MethodID << " for network object type \'KVStore\'\n";
                response.Code = ServerCode::FAIL;
                response.Body = err.str();
                std::cerr << response.Body;
                break;
            }
        }
    }
  private:
    KVStore* _base;
};

class KVStoreClient : public ClientObj {
  public:
    KVStoreClient(std::string name, NOSClient* client, std::string address, uint32_t port)
                        : ClientObj(name, client, address, port) {}

    ~KVStoreClient() {
        _client->mark_obj_deleted(_name);
    }

    bool put(std::string key, std::string value) {
        auto args = std::make_tuple(key, value);
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(KVStoreMethodID::put),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
        return Serializer::unpack<bool>(response.Body);
    }

    std::string get(std::string key) {
        auto args = std::make_tuple(key);
        RPCRequest request(static_cast<uint32_t>(RequestType::invoke), _name,
                           static_cast<uint32_t>(KVStoreMethodID::get),
                           Serializer::pack<decltype(args)>(args));
        RPCResponse response = _client->rpc_send(request, _address, _port);
        handle_rpc_exceptions(response);
        return Serializer::unpack<std::string>(response.Body);
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

#endif /* _KVSTORE_GENERATED_HPP */
