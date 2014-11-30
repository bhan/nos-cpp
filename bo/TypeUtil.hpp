#ifndef _TYPE_UTIL_HPP
#define _TYPE_UTIL_HPP

#include <string>
#include <typeinfo>
#include <unordered_map>

#include "NetObj.hpp"

class NOSAgent;
class TypeUtil {
 public:
  TypeUtil();
  ClientObj* getClientObjFromAgentName(const std::string type_name,
                                       const std::string name,
                                       std::string ip_addr, int port) const;
  AgentObj* getAgentObjForServerObj(NetObj* obj, std::string name,
                                    NOSAgent* agent);
 private:
  typedef ClientObj* (* ClientConstructFunc)(std::string, std::string, int);
  typedef AgentObj* (* AgentConstructFunc)(NetObj*, std::string, NOSAgent*);
  std::unordered_map<std::string, ClientConstructFunc> _client_nameToFunc;
  std::unordered_map<std::string, AgentConstructFunc> _agent_nameToFunc;
};

#endif /* _TYPE_UTIL_H */
