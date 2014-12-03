// USER WRITES THIS
#ifndef _BASE_A_H
#define _BASE_A_H

#include <cstdint>
#include <iostream>
#include "../nos/NetObj.hpp"

class BaseA : public NetObj {
  public:
    BaseA() {}
    BaseA(int32_t num) : _num(num) {}
    virtual ~BaseA() {}

    int32_t decrement() {
      std::cout << "BaseA decrement() called" << std::endl;
      return --_num;
    }
    int32_t increment() {
      std::cout << "BaseA increment() called" << std::endl;
      return ++_num;
    }
  private:
    int32_t _num;
};

#endif /* _BASE_A_H */
