#include <string>
#include <typeinfo>
#include <unordered_map>
#include "../nos/NOSClientTypeUtil.hpp"

#include "KVStoreGenerated.hpp"



ClientObj* constructKVStoreClient(std::string name, NOSClient* client, std::string address, int port) {
    return new KVStoreClient(name, client, address, port);
}


NOSClientTypeUtil::NOSClientTypeUtil() {

    _client_nameToFunc[typeid(KVStoreAgent).name()] = &constructKVStoreClient; // AUTO

}

ClientObj* NOSClientTypeUtil::getClientObjFromAgentName(
    const std::string type_name, const std::string name, NOSClient* client,
    std::string address, int port) const {
    const auto got = _client_nameToFunc.find(type_name);
    if (got == _client_nameToFunc.end()) {
        return nullptr;
    }
    const auto func = got->second;
    return func(name, client, address, port); // call the specific constructor
}
