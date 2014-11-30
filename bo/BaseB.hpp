// USER WRITES THIS
#ifndef _BASE_B_H
#define _BASE_B_H

#include <cstdint>
#include "NetObj.hpp"

class BaseB : public NetObj {
  public:
    BaseB() {}
    BaseB(int32_t num) : _num(num) {}
    virtual ~BaseB() {}

    int32_t increment(int a) {
      for (int i = 0; i < a; ++i) ++_num;
      return _num;
    }
    int32_t decrement(int a) {
      for (int i = 0; i < a; ++i) --_num;
      return _num;
    }

    std::string bar;
    double baz;

  private:
    int32_t _num;
};

#endif /* _BASE_B_H */
