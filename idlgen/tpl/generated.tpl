// AUTO GENERATED
#ifndef _{{CLASS_UPPERCASE}}_GENERATED_H
#define _{{CLASS_UPPERCASE}}_GENERATED_H

#include <arpa/inet.h>
#include <iostream>
#include <stdexcept>

#include "{{CLASS_NAME}}.hpp"
#include "ClientObj.hpp"
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

class {{CLASS_NAME}}Server : public {{CLASS_NAME}} {
 public:
  {{#CTORS}}
  {{CLASS_NAME}}Server({{CTOR_ARGS_WITH_TYPES}}) : _base(new {{CLASS_NAME}}({{CTOR_ARGS}})) {}
  {{/CTORS}}
  {{#METHOD_IMPLS}}
  {{METHOD_RET_TYPE}} {{METHOD_NAME}}({{METHOD_ARGS_WITH_TYPES}}) {
    return _base->{{METHOD_NAME}}({{METHOD_ARGS}});
  }
  {{/METHOD_IMPLS}}
  void dispatch(char* buf, char*& res_buf, uint32_t& res_buf_size) {
    std::cout << "{{CLASS_NAME}}Server dispatch() called" << std::endl;

    int32_t method_code;
    size_t start = Serialize::unpack(buf, 0, method_code);
    switch (static_cast<{{CLASS_NAME}}MethodCode>(method_code)) {
      {{#METHOD_IMPLS}}
      case {{CLASS_NAME}}MethodCode::{{METHOD_NAME}}: {
        std::cout << "dispatch: {{METHOD_NAME}}" << std::endl;
        int32_t a;
        start = Serialize::unpack(buf, start, a);
        int32_t res = this->{{METHOD_NAME}}({{METHOD_ARGS}});
        res_buf_size = Serialize::size(res);
        res_buf = new char[res_buf_size];
        start = Serialize::pack(res_buf, 0, res);
        break;
      }
      {{/METHOD_IMPLS}}
      default: {
        std::cout << "dispatch: unsupported" << std::endl;
        break;
      }
    }
  }
 private:
  {{CLASS_NAME}}* _base;
};

class {{CLASS_NAME}}Client : public {{CLASS_NAME}}, public ClientObj {
 public:
  {{CLASS_NAME}}Client(std::string name, std::string ip_addr, int port) {
    ClientObj::name = name;
    ClientObj::ip_addr = ip_addr;
    ClientObj::port = port;
  }
  {{#METHOD_IMPLS}}
  {{METHOD_RET_TYPE}} {{METHOD_NAME}}({{METHOD_ARGS_WITH_TYPES}}) {
    int32_t return_res; bool error = false;
    size_t start = 0;
    ssize_t sent_ttl; ssize_t sent; ssize_t recvd; ssize_t recvd_ttl;
    int32_t buf_size_wire;
    int32_t recv_buf_size; char* recv_buf;
    char message_code = static_cast<char>(MessageCode::invoke);
    int32_t method_code = static_cast<int32_t>({{CLASS_NAME}}MethodCode::{{METHOD_NAME}});
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
  {{/METHOD_IMPLS}}
 private:
  TCPConnector* connector = new TCPConnector();
};

#endif /* _BASE_B_GENERATED_H */
