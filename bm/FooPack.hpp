#pragma once
#include <random>
#include <unordered_map>
#include "SerializableClosure.hpp"
#include "NOSDaemon.hpp"
#include "NOSClient.hpp"

class Foo;
class FooClientProxy;
class FooServerProxy;

class Foo {
  public:
    int64_t baz;
    double bar_method(const std::string &str, bool t);

  private:
    std::mt19937 mt = std::mt19937(1729);
    std::uniform_real_distribution<double> dist = std::uniform_real_distribution<double>(0, 1000000000);

};

class FooClientProxy : private NOSClient {
  public:
    FooClientProxy(const std::string &address, uint32_t port);
    ~FooClientProxy();

    uint32_t getID();
    void set_baz(int64_t val);
    int64_t get_baz();
    double bar_method(const std::string &str, bool t);

    static FooClientProxy get(uint32_t oid);

  private:
    uint32_t _id;

    static uint32_t create();
    static void del(uint32_t oid);
};

class FooServerProxy : public NOSDaemon {
  public:
    static std::unordered_map<uint32_t, Foo> OBJECT_MAP;

    static uint32_t create();
    static Foo get(uint32_t object_id);
    static void del(uint32_t object_id);

    static RPCResponse process_request(const RPCRequest &request);

    FooServerProxy(uint32_t port);
    void handleRequest();
};
