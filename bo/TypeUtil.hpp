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
  typedef NetObj* (TypeUtil::*ConstructFunc)() const;
  TypeUtil() {
    _nameToFunc[typeid(BaseA).name()] = &TypeUtil::constructBaseAClient;
    _nameToFunc[typeid(BaseB).name()] = &TypeUtil::constructBaseBClient;
  }
  /* Returns an instance of nameClient from name */
  NetObj* getClientObjFromName(const std::string name) const {
    const auto got = _nameToFunc.find(name);
    if (got == _nameToFunc.end()) {
      return NULL;
    }
    const auto func = got->second;
    return ((*this).*func)(); // call the specific constructor
  }
 private:
  std::unordered_map<std::string, ConstructFunc> _nameToFunc;
  NetObj* constructBaseAClient() const { // AUTO
    return new BaseAClient();
  }
  NetObj* constructBaseBClient() const { // AUTO
    return new BaseBClient();
  }
};

#endif /* _TYPE_UTIL_H */
