#pragma once

#include <string>
#include <typeinfo>
#include <unordered_map>

#include "NetObj.hpp"

class NOSClient;
class NOSClientTypeUtil {
 public:
  NOSClientTypeUtil();
  ClientObj* getClientObjFromAgentName(const std::string type_name,
                                       const std::string name,
                                       NOSClient* client,
                                       std::string address,
                                       int port) const;
 private:
  typedef ClientObj* (* ClientConstructFunc)(std::string, NOSClient*,
                                             std::string, int);
  std::unordered_map<std::string, ClientConstructFunc> _client_nameToFunc;
};
