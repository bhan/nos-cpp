#ifndef _AGENT_H
#define _AGENT_H

#include <arpa/inet.h>
#include <cassert>
#include <iostream>
#include <mutex>
#include <functional>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>

#include "NetObj.hpp"
#include "TypeUtil.hpp"
#include "../tcpsockets/tcpacceptor.h"
#include "../tcpsockets/tcpconnector.h"
#include "../tcpsockets/tcpstream.h"

#pragma pack(4)

#define STREAM_SEND(BUF, SIZE) \
 ( \
   { \
    sent_ttl = 0; sent = 0; \
    while (sent_ttl < SIZE) { \
      sent = stream->send((BUF), (SIZE)); \
      if (sent < 0) { \
        std::cout << "Stream send error" << std::endl; \
        break; \
      } \
      sent_ttl += sent; \
    } \
  } \
 )
#define STREAM_RECV(BUF, SIZE) \
 ( \
   { \
    recvd_ttl = 0; recvd = 0; \
    while (recvd_ttl < SIZE) { \
      recvd = stream->receive((BUF), (SIZE)); \
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
    char message_type; uint32_t name_len;
    ssize_t sent_ttl; ssize_t sent; ssize_t recvd; ssize_t recvd_ttl;
    NetObj* return_res = NULL;

    TCPConnector* connector = new TCPConnector();
    TCPStream* stream = connector->connect(ip_addr.c_str(), port);
    if (stream == NULL) { goto cleanup_connector; }

    message_type = static_cast<char>(MessageType::get_type); // char, so no endian worry
    STREAM_SEND((const char*)&message_type, sizeof(message_type));
    if (sent < 0) { goto cleanup_connector; }

    name_len = htonl(name.size());
    STREAM_SEND((const char*)&name_len, sizeof(name_len));
    if (sent < 0) { goto cleanup_connector; }

    STREAM_SEND(name.c_str(), name.size());
    if (sent < 0) { goto cleanup_connector; }

    uint32_t type_len;
    STREAM_RECV((char*)&type_len, sizeof(type_len));
    if (recvd < 0) { goto cleanup_connector; }
    type_len = ntohl(type_len);
    std::cout << "type_len " << type_len << std::endl;

    if (type_len == 0) {
      std::cout << "No NetObj named " << name << " found" << std::endl;
      goto cleanup_connector;
    }

    {
    char* type = new char[type_len+1];
    STREAM_RECV(type, type_len);
    if (recvd < 0) { goto cleanup_type; }
    type[type_len] = '\0';
    std::cout << name << " of type " << type << std::endl;
    return_res = _type_util.getClientObjFromName(type);
cleanup_type: delete type;
    }
cleanup_connector: delete connector;
    return return_res;
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

    ssize_t sent_ttl; ssize_t sent; ssize_t recvd; ssize_t recvd_ttl;
    while (!should_exit) {
      mtx.unlock();
      TCPStream* stream = acceptor->accept();
      if (stream == NULL) { goto err_exit; }

      char message_type;
      STREAM_RECV(&message_type, sizeof(message_type));
      if (recvd < 0) { goto cleanup_stream; }

      switch (static_cast<MessageType>(message_type)) {
        case MessageType::get_type: {
          std::cout << "get_type" << std::endl;

          uint32_t name_len;
          STREAM_RECV((char*)&name_len, sizeof(name_len));
          if (recvd < 0) { goto cleanup_stream; }
          name_len = ntohl(name_len);
          std::cout << "name_len " << name_len << std::endl;

          char* name = new char[name_len+1];
          STREAM_RECV(name, name_len);
          if (recvd < 0) { goto cleanup_name; }
          name[name_len] = '\0';
          std::cout << "name " << name << std::endl;

          mtx.lock(); {
          auto it = name_to_NetObj.find(name);
          if (it == name_to_NetObj.end()) {
            uint32_t type_len = 0;
            STREAM_SEND((char*)&type_len, sizeof(type_len));
            if (sent < 0) { goto cleanup_get_type_response; }
          } else { // found NetObj for name
            std::string type = typeid(*(it->second)).name();
            uint32_t type_len = htonl(type.size());
            STREAM_SEND((char*)&type_len, sizeof(type_len));
            if (sent < 0) { goto cleanup_get_type_response; }

            STREAM_SEND(type.c_str(), type.size());
            if (sent < 0) { goto cleanup_get_type_response; }
            std::cout << "sent " << name << " of type " << type << std::endl;
          }
          }

cleanup_get_type_response: mtx.unlock();
cleanup_name: delete name;
          break;
        }
        case MessageType::invoke: {
          std::cout << "invoke" << std::endl;
          break;
        }
        default: {
          std::cout << "no valid message type received" << std::endl;
          goto cleanup_stream;
        }
      }
cleanup_stream: delete stream;
err_exit:
      mtx.lock();
    }
    // TODO thread cleanup
  }

  static Agent* _instance;
  std::unordered_map<std::string, NetObj*> _name_to_NetObj;
  std::mutex _mtx;
  volatile bool _should_exit; // tell the detached thread to stop looping
  TypeUtil _type_util;
};

#endif /*  _AGENT_H */
