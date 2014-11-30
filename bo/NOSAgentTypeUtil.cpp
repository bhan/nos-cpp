#include <string>
#include <typeinfo>
#include <unordered_map>

#include "NOSAgentTypeUtil.hpp"
#include "BaseAGenerated.hpp" // AUTO
#include "BaseBGenerated.hpp" // AUTO

AgentObj* constructBaseAAgent(NetObj* obj, std::string name, NOSAgent* agent) {
  return new BaseAAgent(obj, name, agent);
}
AgentObj* constructBaseBAgent(NetObj* obj, std::string name, NOSAgent* agent) {
  return new BaseBAgent(obj, name, agent);
}

NOSAgentTypeUtil::NOSAgentTypeUtil() {
  _agent_nameToFunc[typeid(BaseAServer).name()] = &constructBaseAAgent;
  _agent_nameToFunc[typeid(BaseBServer).name()] = &constructBaseBAgent;
}

AgentObj* NOSAgentTypeUtil::getAgentObjForServerObj(
    NetObj* obj, std::string name, NOSAgent* agent) {
  const std::string type_name = typeid(*obj).name();
  const auto got = _agent_nameToFunc.find(type_name);
  if (got == _agent_nameToFunc.end()) {
    return nullptr;
  }
  const auto func = got->second;
  return func(obj, name, agent); // call the specific constructor
}
