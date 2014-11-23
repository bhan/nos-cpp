// USER WRITES THIS
#ifndef _BASE_A_H
#define _BASE_A_H

#include <cstdint>
#include <iostream>
#include "NetObj.hpp"

class BaseA : public NetObj {
  public:
    BaseA() {}
    virtual ~BaseA() {}

    int increment() {
      std::cout << "BaseA increment() called" << std::endl;
      return ++_num;
    }
  private:
    int32_t _num = 0;
};

#endif /* _BASE_A_H */
