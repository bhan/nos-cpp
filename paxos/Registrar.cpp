#include <string>
#include <typeinfo>
#include <unordered_map>
#include "../nos/NOSAgentTypeUtil.hpp"

#include "AcceptorGenerated.hpp"



AgentObj* constructAcceptorAgent(NetObj* obj, std::string name, NOSAgent* agent) {
    return new AcceptorAgent(obj, name, agent);
}

NOSAgentTypeUtil::NOSAgentTypeUtil() {

    _agent_nameToFunc[typeid(AcceptorServer).name()] = &constructAcceptorAgent;

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
