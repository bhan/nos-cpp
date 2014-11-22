#include "netobj.hpp"

class MyClass : NetObj {
  public:
    int public_int = 0;
    int increment() {
      ++public_int;
    }
    int increment_by(int num) {
      public_int += num;
    }
  private:
    int int1;
    int int2;
    int method1() {
      return int1+int2;
    }
}
