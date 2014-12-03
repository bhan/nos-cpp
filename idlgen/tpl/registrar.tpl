#include <string>
#include <typeinfo>
#include <unordered_map>

#include "../nos/NOSAgentTypeUtil.hpp"

{{#CLASSES}}
#include "{{CLASS_NAME}}Generated.hpp"
{{/CLASSES}}

{{#CLASSES}}
AgentObj* construct{{CLASS_NAME}}Agent(NetObj* obj, std::string name, NOSAgent* agent) {
    return new {{CLASS_NAME}}Agent(obj, name, agent);
}
{{/CLASSES}}
NOSAgentTypeUtil::NOSAgentTypeUtil() {
{{#CLASSES}}
  _agent_nameToFunc[typeid({{CLASS_NAME}}Server).name()] = &construct{{CLASS_NAME}}Agent;
{{/CLASSES}}
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
