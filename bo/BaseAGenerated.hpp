// AUTO GENERATED
#ifndef _BASE_A_GENERATED_H
#define _BASE_A_GENERATED_H

#include <iostream>
#include "BaseA.hpp"

class BaseAWrapper : public BaseA { // used by the agent
  public:
    BaseAWrapper(BaseA& base) : _base(base) {}
    int increment() {
      std::cout << "BaseAWrapper increment() called" << std::endl;
      return _base.increment();
    }
  private:
    BaseA& _base;
};

class BaseAClient : public BaseA {
  public:
    BaseAClient() {}
    int increment() {
      std::cout << "BaseAClient increment() called" << std::endl;
      return 0;
    }
  private:
};

#endif /* _BASE_A_GENERATED_H */
