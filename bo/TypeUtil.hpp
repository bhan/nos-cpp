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

class TypeUtil {
 public:
  typedef NetObj* (TypeUtil::*ConstructFunc)(std::string, std::string, int) const;
  TypeUtil() {
    _nameToFunc[typeid(BaseAServer).name()] = &TypeUtil::constructBaseAClient;
    _nameToFunc[typeid(BaseBServer).name()] = &TypeUtil::constructBaseBClient;
  }
  /* Returns an instance of nameClient from name */
  NetObj* getClientObjFromName(const std::string type_name, const std::string name, std::string ip_addr, int port) const {
    const auto got = _nameToFunc.find(type_name);
    if (got == _nameToFunc.end()) {
      return NULL;
    }
    const auto func = got->second;
    return ((*this).*func)(name, ip_addr, port); // call the specific constructor
  }
 private:
  std::unordered_map<std::string, ConstructFunc> _nameToFunc;
  NetObj* constructBaseAClient(std::string name, std::string ip_addr, int port) const { // AUTO
    return new BaseAClient(name, ip_addr, port);
  }
  NetObj* constructBaseBClient(std::string name, std::string ip_addr, int port) const { // AUTO
    return new BaseBClient(name, ip_addr, port);
  }
};

#endif /* _TYPE_UTIL_H */
