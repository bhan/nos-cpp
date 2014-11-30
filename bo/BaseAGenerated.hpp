#ifndef _BASE_A_GENERATED_H
#define _BASE_A_GENERATED_H

#include <iostream>
#include <string>
#include "NOSAgent.hpp"
#include "BaseA.hpp"
#include "NetObj.hpp"

class BaseAServer : public BaseA { // used by the server
  friend class BaseAAgent;
  public:
    BaseAServer() : _base(new BaseA()) {}
    ~BaseAServer() {
      _agent->mark_obj_deleted(_name);
    }
    int32_t decrement() {
      std::cout << "BaseAServer decrement() called" << std::endl;
      return _base->decrement();
    }
    int32_t increment() {
      std::cout << "BaseAServer increment() called" << std::endl;
      return _base->increment();
    }
 private:
  BaseA* _base;
  NOSAgent* _agent;
  std::string _name;
};

class BaseAAgent: public BaseA, public AgentObj {
 public:
  BaseAAgent(NetObj* obj, std::string name, NOSAgent* agent) {
    BaseAServer* baseAServer = dynamic_cast<BaseAServer*>(obj);
    _base = baseAServer->_base;
    baseAServer->_name = name;
    baseAServer->_agent = agent;
  }
  void dispatch(RPCRequest& request, RPCResponse& response) {
    std::cout << "BaseAServer dispatch() called" << std::endl;
  }
 private:
  BaseA* _base;
};

class BaseAClient : public ClientObj {
 public:
  BaseAClient(std::string name, NOSClient* client,
              std::string address, uint32_t port)
    : ClientObj(name, client, address, port) {}
  int32_t decrement() {
    return 0;
  }
  int32_t increment() {
    return 0;
  }
};
#endif /* _BASE_A_GENERATED_H */
