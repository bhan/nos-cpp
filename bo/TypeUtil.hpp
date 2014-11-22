/* AUTO GENERATED */
#ifndef _TYPE_UTIL_H
#define _TYPE_UTIL_H

#include <unordered_map>

#include "Base.hpp" // AUTO
#include "BaseGenerated.hpp" // AUTO

class TypeUtil {
  public:
    static NetObj GetClientObjFromName() {
    }
  private:
    static StaticTypeInfo _type_info ;
    class StaticTypeInfo {
      friend class TypeUtil;
      public:
        StaticTypeInfo() {
        }
      private:
        std::unordered_map BaseNameToClientConstructorMethod;
        BaseClient* constructBaseClient() {
          return new BaseClient();
        }
    };
};

#endif /* _TYPE_UTIL_H */
