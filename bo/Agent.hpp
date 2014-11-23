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


class Agent {
 public:
  static Agent* Instance() {
    if (_instance == nullptr) {
      _instance = new Agent();
    }
    return _instance;
  }
  void Initialize() { // don't call more than once
    _mtx.lock();
    _should_exit = false;
    std::thread(_run, std::ref(_mtx), std::ref(_name_to_NetObj),
        std::ref(_should_exit)).detach();
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
      bool& should_exit) {
    // thread initialization
    mtx.unlock();
    mtx.lock();
    while (!should_exit) {
      mtx.unlock();
      // TODO add message receiver for Import and dispatch
    }
    // TODO cleanup
  }

  static Agent* _instance;
  std::unordered_map<std::string, NetObj*> _name_to_NetObj;
  std::mutex _mtx;
  bool _should_exit; // tell the detached thread to stop looping
  std::mutex _print_lock;
};

#endif /*  _AGENT_H */
