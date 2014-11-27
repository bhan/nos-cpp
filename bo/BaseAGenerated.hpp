// AUTO GENERATED
#ifndef _BASE_A_GENERATED_H
#define _BASE_A_GENERATED_H

#include <iostream>
#include <string>
#include "BaseA.hpp"
#include "ClientObj.hpp"

class BaseAServer : public BaseA { // used by the server
  public:
    BaseAServer() : _base(new BaseA()) {}
    int32_t decrement() {
      std::cout << "BaseAServer increment() called" << std::endl;
      return _base->increment();
    }
    int32_t increment() {
      std::cout << "BaseAServer increment() called" << std::endl;
      return _base->increment();
    }
    char* dispatch(char* buf) {
      std::cout << "BaseAServer dispatch() called" << std::endl;
      return nullptr;
    }
  private:
    BaseA* _base;
};

class BaseAClient : public BaseA, public ClientObj {
  public:
    BaseAClient(std::string name, std::string ip_addr, int port) {
      ClientObj::name = name;
      ClientObj::ip_addr = ip_addr;
      ClientObj::port = port;
    }
    int32_t decrement() {
      return 0;
    }
    int32_t increment() {
      return 0;
    }
  private:
};

#endif /* _BASE_A_GENERATED_H */
