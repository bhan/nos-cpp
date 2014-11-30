#pragma once

#include <string>
#include <typeinfo>
#include <unordered_map>

#include "NetObj.hpp"

class NOSAgent;
class NOSAgentTypeUtil {
 public:
  NOSAgentTypeUtil();
  AgentObj* getAgentObjForServerObj(NetObj* obj, std::string name,
                                    NOSAgent* agent);
 private:
  typedef AgentObj* (* AgentConstructFunc)(NetObj*, std::string, NOSAgent*);
  std::unordered_map<std::string, AgentConstructFunc> _agent_nameToFunc;
};
