#include <string>
#include <typeinfo>
#include <unordered_map>

#include "TypeUtil.hpp"
#include "BaseAGenerated.hpp" // AUTO
#include "BaseBGenerated.hpp" // AUTO

ClientObj* constructBaseAClient(std::string name, std::string ip_addr, int port) { // AUTO
  return new BaseAClient(name, ip_addr, port);
}
ClientObj* constructBaseBClient(std::string name, std::string ip_addr, int port) { // AUTO
  return new BaseBClient(name, ip_addr, port);
}
AgentObj* constructBaseAAgent(NetObj* obj, std::string name, Agent* agent) {
  return new BaseAAgent(obj, name, agent);
}
AgentObj* constructBaseBAgent(NetObj* obj, std::string name, Agent* agent) {
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
                                               std::string ip_addr,
                                               int port) const {
  const auto got = _client_nameToFunc.find(type_name);
  if (got == _client_nameToFunc.end()) {
    return nullptr;
  }
  const auto func = got->second;
  return func(name, ip_addr, port); // call the specific constructor
}
AgentObj* TypeUtil::getAgentObjForServerObj(NetObj* obj, std::string name,
                                            Agent* agent) {
  const std::string type_name = typeid(*obj).name();
  const auto got = _agent_nameToFunc.find(type_name);
  if (got == _agent_nameToFunc.end()) {
    return nullptr;
  }
  const auto func = got->second;
  return func(obj, name, agent); // call the specific constructor
}
