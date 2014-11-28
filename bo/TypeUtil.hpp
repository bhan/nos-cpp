/* AUTO GENERATED */
#ifndef _TYPE_UTIL_H
#define _TYPE_UTIL_H

#include <string>
#include <typeinfo>
#include <unordered_map>

#include "BaseA.hpp" // AUTO
#include "BaseAGenerated.hpp" // AUTO
#include "BaseB.hpp" // AUTO
#include "BaseBGenerated.hpp" // AUTO

class Agent;
class TypeUtil {
 public:
  typedef ClientObj* (TypeUtil::*ClientConstructFunc)(std::string, std::string, int) const;
  typedef AgentObj* (TypeUtil::*AgentConstructFunc)(NetObj*, std::string, Agent*) const;
  TypeUtil() {
    _client_nameToFunc[typeid(BaseAAgent).name()] = &TypeUtil::constructBaseAClient;
    _client_nameToFunc[typeid(BaseBAgent).name()] = &TypeUtil::constructBaseBClient;
    _agent_nameToFunc[typeid(BaseAServer).name()] = &TypeUtil::constructBaseAAgent;
    _agent_nameToFunc[typeid(BaseBServer).name()] = &TypeUtil::constructBaseBAgent;
  }
  /* Returns an instance of nameClient from name indicating AgentObj type */
  ClientObj* getClientObjFromAgentName(const std::string type_name, const std::string name, std::string ip_addr, int port) const {
    const auto got = _client_nameToFunc.find(type_name);
    if (got == _client_nameToFunc.end()) {
      return NULL;
    }
    const auto func = got->second;
    return ((*this).*func)(name, ip_addr, port); // call the specific constructor
  }
  AgentObj* getAgentObjForServerObj(NetObj* obj, std::string name, Agent* agent) {
    const std::string type_name = typeid(*obj).name();
    const auto got = _agent_nameToFunc.find(type_name);
    if (got == _agent_nameToFunc.end()) {
      return NULL;
    }
    const auto func = got->second;
    return ((*this).*func)(obj, name, agent); // call the specific constructor
  }
 private:
  std::unordered_map<std::string, ClientConstructFunc> _client_nameToFunc;
  std::unordered_map<std::string, AgentConstructFunc> _agent_nameToFunc;
  ClientObj* constructBaseAClient(std::string name, std::string ip_addr, int port) const { // AUTO
    return new BaseAClient(name, ip_addr, port);
  }
  ClientObj* constructBaseBClient(std::string name, std::string ip_addr, int port) const { // AUTO
    return new BaseBClient(name, ip_addr, port);
  }
  AgentObj* constructBaseAAgent(NetObj* obj, std::string name, Agent* agent) const {
    return new BaseAAgent(obj, name, agent);
  }
  AgentObj* constructBaseBAgent(NetObj* obj, std::string name, Agent* agent) const {
    return new BaseBAgent(obj, name, agent);
  }
};

#endif /* _TYPE_UTIL_H */
