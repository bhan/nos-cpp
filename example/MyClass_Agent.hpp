// these objects are stored in the agent on the server
class MyClass_Agent : MyClass {
  public:
    MyClass_Agent(MyClass myClass) : myClass_(myClass) {
    }
    int increment(char* buffer, int size) {
      return myClass_.increment();
    }
    int increment_by(char* buffer, int size) {
      int pos = 0;
      int i1;
      pos = deserialize(buffer, pos, &i1); // repeat for each argument
      return myClass_.increment_by(i1);
    }
  private:
    MyClass myClass_;
}
