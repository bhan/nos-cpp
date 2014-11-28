// AUTO GENERATED
#ifndef _BASE_B_GENERATED_H
#define _BASE_B_GENERATED_H

#include <arpa/inet.h>
#include <iostream>
#include <stdexcept>

#include "Agent.hpp"
#include "BaseB.hpp"
#include "NetObj.hpp"
#include "Codes.hpp"
#include "Serialize.hpp"
#include "../tcpsockets/tcpconnector.h"
#include "../tcpsockets/tcpstream.h"

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

class BaseBServer : public BaseB {
  friend class BaseBAgent;
 public:
  BaseBServer(int32_t num) : _base(new BaseB(num)) {}
  ~BaseBServer() {
    _agent->mark_obj_deleted(_name);
  }
  int32_t decrement(int32_t a) {
    return _base->decrement(a);
  }
  int32_t increment(int32_t a) {
    return _base->decrement(a);
  }
 private:
  BaseB* _base;
  Agent* _agent;
  std::string _name;
};

class BaseBAgent : public AgentObj {
 public:
  BaseBAgent(NetObj* obj, std::string name, Agent* agent) {
    BaseBServer* baseBServer = dynamic_cast<BaseBServer*>(obj);
    _base = baseBServer->_base;
    baseBServer->_name = name;
    baseBServer->_agent = agent;
  }
  void dispatch(char* buf, char*& res_buf, uint32_t& res_buf_size) {
    std::cout << "BaseAServer dispatch() called" << std::endl;

    int32_t method_code;
    size_t start = Serialize::unpack(buf, 0, method_code);
    switch (static_cast<BaseBMethodCode>(method_code)) {
      case BaseBMethodCode::decrement: {
        std::cout << "dispatch: decrement" << std::endl;
        int32_t a;
        start = Serialize::unpack(buf, start, a);
        int32_t res = _base->decrement(a);
        res_buf_size = Serialize::size(res);
        res_buf = new char[res_buf_size];
        start = Serialize::pack(res_buf, 0, res);
        break;
      }
      default: {
        std::cout << "dispatch: unsupported" << std::endl;
        break;
      }
    }
  }
 private:
  BaseB* _base;
};

class BaseBClient : public BaseB, public ClientObj {
 public:
  BaseBClient(std::string name, std::string ip_addr, int port) {
    ClientObj::name = name;
    ClientObj::ip_addr = ip_addr;
    ClientObj::port = port;
  }
  int32_t decrement(int32_t a) {
    int32_t return_res; bool error = false;
    size_t start = 0;
    ssize_t sent_ttl; ssize_t sent; ssize_t recvd; ssize_t recvd_ttl;
    int32_t buf_size_wire;
    int32_t recv_buf_size; char* recv_buf;
    char message_code = static_cast<char>(MessageCode::invoke);
    int32_t method_code = static_cast<int32_t>(BaseBMethodCode::decrement);
    int32_t buf_size = Serialize::size(ClientObj::name) + Serialize::size(method_code);
    buf_size += Serialize::size(a);
    char* buf = new char[buf_size];

    TCPStream* stream = connector->connect(ClientObj::ip_addr.c_str(),
                                           ClientObj::port);

    STREAM_SEND(&message_code, sizeof(message_code));
    if (sent < 0) { error = true; goto cleanup_buf; }

    buf_size_wire = htonl(buf_size);
    STREAM_SEND((char*)&buf_size_wire, sizeof(buf_size_wire));
    if (sent < 0) { error = true; goto cleanup_buf; }

    start = Serialize::pack(buf, start, ClientObj::name);
    start = Serialize::pack(buf, start, method_code);
    start = Serialize::pack(buf, start, a);

    STREAM_SEND(buf, buf_size);
    if (sent < 0) { error = true; goto cleanup_buf; }

    STREAM_RECV((char*)&recv_buf_size, sizeof(recv_buf_size));
    if (recvd < 0) { error = true; goto cleanup_buf; }
    recv_buf_size = ntohl(recv_buf_size);

    recv_buf = new char[recv_buf_size];
    STREAM_RECV(recv_buf, recv_buf_size);
    if (recvd < 0) { error = true; goto cleanup_recv_buf; }

    Serialize::unpack(recv_buf, 0, return_res);
cleanup_recv_buf: delete[] recv_buf;
cleanup_buf: delete[] buf;
    return error ? throw std::runtime_error("network error") : return_res;
  }

  int32_t increment(int32_t a) {
    const int32_t method_code = static_cast<int32_t>(BaseBMethodCode::increment);
    int32_t buf_size = Serialize::size(ClientObj::name) + Serialize::size(method_code);
    buf_size += Serialize::size(a);
    return 0;
  }
 private:
  TCPConnector* connector = new TCPConnector();
};

#endif /* _BASE_B_GENERATED_H */
