#include <string>
#include <typeinfo>
#include <unordered_map>

#include "TypeUtil.hpp"
#include "BaseAGenerated.hpp" // AUTO
#include "BaseBGenerated.hpp" // AUTO

ClientObj* constructBaseAClient(std::string name, NOSClient* client, std::string address, int port) {
  return new BaseAClient(name, client, address, port);
}
ClientObj* constructBaseBClient(std::string name, NOSClient* client, std::string address, int port) {
  return new BaseBClient(name, client, address, port);
}
AgentObj* constructBaseAAgent(NetObj* obj, std::string name, NOSAgent* agent) {
  return new BaseAAgent(obj, name, agent);
}
AgentObj* constructBaseBAgent(NetObj* obj, std::string name, NOSAgent* agent) {
  return new BaseBAgent(obj, name, agent);
}

TypeUtil::TypeUtil() {
  _client_nameToFunc[typeid(BaseAAgent).name()] = &constructBaseAClient;
  _client_nameToFunc[typeid(BaseBAgent).name()] = &constructBaseBClient;
  _agent_nameToFunc[typeid(BaseAServer).name()] = &constructBaseAAgent;
  _agent_nameToFunc[typeid(BaseBServer).name()] = &constructBaseBAgent;
}
ClientObj* TypeUtil::getClientObjFromAgentName(const std::string type_name,
                                               const std::string name,
                                               NOSClient* client,
                                               std::string address,
                                               int port) const {
  const auto got = _client_nameToFunc.find(type_name);
  if (got == _client_nameToFunc.end()) {
    return nullptr;
  }
  const auto func = got->second;
  return func(name, client, address, port); // call the specific constructor
}
AgentObj* TypeUtil::getAgentObjForServerObj(NetObj* obj, std::string name,
                                            NOSAgent* agent) {
  const std::string type_name = typeid(*obj).name();
  const auto got = _agent_nameToFunc.find(type_name);
  if (got == _agent_nameToFunc.end()) {
    return nullptr;
  }
  const auto func = got->second;
  return func(obj, name, agent); // call the specific constructor
}
