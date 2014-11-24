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
#include "../tcpsockets/tcpconnector.h"
#include "../tcpsockets/tcpstream.h"

#pragma pack(4)

#define STREAM_SEND(M) \
 ( \
   { \
    sent_ttl = 0; sent = 0; \
    while (sent_ttl < sizeof((M))) { \
      sent = stream->send((const char*)&(M), sizeof((M))); \
      if (sent < 0) { \
        std::cout << "Stream send error" << std::endl; \
        break; \
      } \
      sent_ttl += sent; \
    } \
  } \
 )
#define STREAM_RECV(M) \
 ( \
   { \
    recvd_ttl = 0; recvd = 0; \
    while (recvd_ttl < sizeof((M))) { \
      recvd = stream->receive((char*)&(M), sizeof((M))); \
      if (recvd < 0) { \
        std::cout << "Stream recv error" << std::endl; \
        break; \
      } \
      recvd_ttl += recvd; \
    } \
  } \
 )

enum class MessageType : char { invalid, get_type, invoke };

class Agent {
 public:
  static Agent* Instance() {
    if (_instance == nullptr) {
      _instance = new Agent();
    }
    return _instance;
  }
  void Initialize(int port, std::string ip_addr) { // don't call more than once
    _should_exit = false;
    std::thread(_run, std::ref(_mtx), std::ref(_name_to_NetObj),
        std::ref(_should_exit), port, std::ref(ip_addr)).detach();
  }
  void Exit() { // assumes Initialize was called, don't call more than once
    _mtx.lock();
    _should_exit = true;
    _mtx.unlock();
  }
  NetObj* Import(std::string name, int port, std::string ip_addr) {
    TCPConnector* connector = new TCPConnector();
    TCPStream* stream = connector->connect(ip_addr.c_str(), port);
    char message_type; ssize_t sent_ttl; ssize_t sent;
    if (stream == NULL) { goto cleanup_connector; }

    message_type = static_cast<char>(MessageType::get_type); // char, so no endian worry
    STREAM_SEND(message_type);
    if (sent < 0) {
      goto cleanup_connector;
    }
cleanup_connector:
    delete connector;
    return NULL;
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
      volatile bool& should_exit, int port, std::string& ip_addr) {
    // thread holds lock here
    // thread initialization
    TCPAcceptor* acceptor = new TCPAcceptor(port, ip_addr.c_str());
    if (acceptor->start() != 0) {
      std::cout << "Agent failed to start TCP acceptor" << std::endl;
      mtx.unlock();
      exit(1);
    }
    std::cout << "Agent started at " << ip_addr << ":" << port << std::endl;

    char message_type;
    ssize_t recvd;
    ssize_t recvd_ttl;
    while (!should_exit) {
      mtx.unlock();
      // TODO add message receiver for Import and dispatch
      TCPStream* stream = acceptor->accept();
      if (stream == NULL) { goto err_exit; }

      STREAM_RECV(message_type);
      if (recvd < 0) {
        goto cleanup_stream;
      }

      switch (static_cast<MessageType>(message_type)) {
        case MessageType::get_type:
          std::cout << "get_type" << std::endl;
          break;
        case MessageType::invoke: //
          std::cout << "invoke" << std::endl;
          break;
        default: {
          std::cout << "no valid message type received" << std::endl;
          goto cleanup_stream;
        }
      }
cleanup_stream: delete stream;
err_exit:
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
