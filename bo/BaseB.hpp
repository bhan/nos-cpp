// USER WRITES THIS
#ifndef _BASE_B_H
#define _BASE_B_H

#include <cstdint>
#include "NetObj.hpp"

class BaseB : public NetObj {
  public:
    BaseB() {}
    virtual ~BaseB() {}

    int decrement() {
      return --_num;
    }
  private:
    int32_t _num = 0;
};

#endif /* _BASE_B_H */
