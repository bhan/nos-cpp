#ifndef _AGENT_H
#define _AGENT_H

#include <cassert>
#include <iostream>
#include <mutex>
#include <functional>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>

#include "NetObj.hpp"
#include "../tcpsockets/tcpacceptor.h"


class Agent {
 public:
  static Agent* Instance() {
    if (_instance == nullptr) {
      _instance = new Agent();
    }
    return _instance;
  }
  void Initialize(int port, std::string ip_addr) { // don't call more than once
    _mtx.lock();
    _should_exit = false;
    std::thread(_run, std::ref(_mtx), std::ref(_name_to_NetObj),
        std::ref(_should_exit), std::ref(port), std::ref(ip_addr)).detach();
  }
  void Exit() { // don't call more than once
    _mtx.lock();
    _should_exit = true;
    _mtx.unlock();
  }
  void Export(const std::string name, NetObj* netObj) {
    _mtx.lock();
    if (!(_name_to_NetObj.find(name) == _name_to_NetObj.end())) {
      _mtx.unlock();
      throw std::runtime_error(name + "already assigned");
    }
    _name_to_NetObj[name] = netObj; // TODO need GC info here
//    std::cout << typeid(*netObj).name() << " " << name << " inserted"
//      << std::endl;
    _mtx.unlock();
  }
  void PrintExported() {
    _mtx.lock();
    for (auto it = _name_to_NetObj.begin(); it != _name_to_NetObj.end(); ++it) {
      std::cout << typeid(*(it->second)).name() << " " << it->first
        << std::endl;
    }
    _mtx.unlock();
  }
 private:
  Agent() {}
  Agent(const Agent&) {}
  Agent& operator=(const Agent&) { return *_instance; }

  static void _run(std::mutex& mtx,
      std::unordered_map<std::string, NetObj*>& name_to_NetObj,
      volatile bool& should_exit, int& port, std::string& ip_addr) {
    // thread holds lock here
    // thread initialization
    TCPAcceptor* acceptor = new TCPAcceptor(port, ip_addr.c_str());
    if (acceptor->start() != 0) {
      std::cout << "Agent failed to start TCP acceptor" << std::endl;
      mtx.unlock();
      exit(1);
    }
    std::cout << "Agent started at " << ip_addr << ":" << port << std::endl;

    while (!should_exit) {
      mtx.unlock();
      // TODO add message receiver for Import and dispatch
      // read message from tcp socket
      mtx.lock();
    }
    // TODO cleanup
  }

  static Agent* _instance;
  std::unordered_map<std::string, NetObj*> _name_to_NetObj;
  std::mutex _mtx;
  volatile bool _should_exit; // tell the detached thread to stop looping
};

#endif /*  _AGENT_H */
