#include "serialize.h"

class MyClass_Client : MyClass {
  public:
    MyClass_Client() :  {}
    int increment() {
      char* res, int res_size = agent.make_call(0, nullptr, 0); // 0th arg is method index, which agent uses to dispatch right method
      int result;
      deserialize(res, 0, &result);
      return result;
    }
    int increment_by(int num) {
      int size = SERIALIZED_INT_SIZE; // add on each argument type
      char* buffer = new char[size];
      int pos = 0;
      pos = serialize_to_buffer(buffer, pos, SERIALIZED_INT_SIZE, num); // adds to buffer at pos, repeat for all arguments
      char* res, int res_size = agent.make_call(1, buffer, size); // 0th arg is method index
      int result;
      deserialize(res, 0, &result);
      return result;
    }

    // need to override dot operator??? to mutate public fields in MyClass?
  private:
    NetObj.ClientAgent agent; // some reference to agent that maintains server
}
