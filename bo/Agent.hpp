class Agent {
  public:
    static Agent* Instance() {
      if (_instance == nullptr) {
        _instance = new Agent();
      }
      return _instance;
    }
    void initialize() {
    }

  private:
    Agent() {}
    Agent(const Agent&) {}
    Agent& operator=(const Agent&) { return *_instance; }
    static Agent* _instance;
};
