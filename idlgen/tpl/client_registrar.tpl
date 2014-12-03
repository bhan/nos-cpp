#include <string>
#include <typeinfo>
#include <unordered_map>

#include "../nos/NOSClientTypeUtil.hpp"

{{#CLASSES}}
#include "{{CLASS_NAME}}Generated.hpp"
{{/CLASSES}}

{{#CLASSES}}
ClientObj* construct{{CLASS_NAME}}Client(std::string name, NOSClient* client, std::string address, int port) {
    return new {{CLASS_NAME}}Client(name, client, address, port);
}
{{/CLASSES}}

NOSClientTypeUtil::NOSClientTypeUtil() {
{{#CLASSES}}
  _client_nameToFunc[typeid({{CLASS_NAME}}Agent).name()] = &construct{{CLASS_NAME}}Client; // AUTO
{{/CLASSES}}
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
