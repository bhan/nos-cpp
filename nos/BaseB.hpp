// USER WRITES THIS
#ifndef _BASE_B_H
#define _BASE_B_H

#include <cstdint>
#include <vector>
#include "../nos/NetObj.hpp"

class BaseB : public NetObj {
  public:
    BaseB() {}
    BaseB(int32_t num) : _num(num) {}
    virtual ~BaseB() {}

    int32_t increment(int32_t a) {
      for (int i = 0; i < a; ++i) ++_num;
      return _num;
    }
    int32_t decrement(int a) {
      for (int i = 0; i < a; ++i) --_num;
      return _num;
    }

    bool bork_method(uint32_t a, std::string b) {
      return true;
    }

    void bark_method(std::vector<double> a) {
      return;
    }

    void void_method() {
      return;
    }

    std::vector<std::string> bar;
    double baz;


  private:
    int32_t _num;
};

#endif /* _BASE_B_H */
