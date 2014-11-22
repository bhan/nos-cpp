// AUTO GENERATED
#ifndef _BASE_B_GENERATED_H
#define _BASE_B_GENERATED_H

#include "BaseB.hpp"

class BaseBWrapper : public BaseB { // used by the agent
 public:
  BaseBWrapper(BaseB& base) : _base(base) {}
  int decrement() {
    return _base.decrement();
  }
 private:
  BaseB& _base;
};

class BaseBClient : public BaseB {
 public:
  BaseBClient() {}
  int decrement() {
    return 0;
  }
 private:
};

#endif /* _BASE_B_GENERATED_H */
