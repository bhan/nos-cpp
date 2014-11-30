#include <string>
#include <typeinfo>
#include <unordered_map>

#include "NOSClientTypeUtil.hpp"
#include "BaseAGenerated.hpp" // AUTO
#include "BaseBGenerated.hpp" // AUTO

ClientObj* constructBaseAClient(std::string name, NOSClient* client, std::string address, int port) {
  return new BaseAClient(name, client, address, port);
}
ClientObj* constructBaseBClient(std::string name, NOSClient* client, std::string address, int port) {
  return new BaseBClient(name, client, address, port);
}

NOSClientTypeUtil::NOSClientTypeUtil() {
  _client_nameToFunc[typeid(BaseAAgent).name()] = &constructBaseAClient;
  _client_nameToFunc[typeid(BaseBAgent).name()] = &constructBaseBClient;
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
