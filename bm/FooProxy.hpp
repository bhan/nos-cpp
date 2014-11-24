#include <random>

class FooProxy {
public:
    FooProxy(int32_t x);
    double bar_method(const std::string &str, bool t);

private:
    uint32_t _x;
    std::mt19937 mt = std::mt19937(1729);
    std::uniform_real_distribution<double> dist = std::uniform_real_distribution<double>(0, 1000000000);
};
