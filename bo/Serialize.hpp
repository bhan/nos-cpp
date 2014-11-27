// WE WRITE THIS
#ifndef _SERIALIZE_HPP
#define _SERIALIZE_HPP

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <string>

class Serialize {
 public:
  static size_t size(char& arg) {
    return sizeof(char);
  }
  static size_t pack(char* buf, size_t buf_start, char& arg) {
    memcpy(buf+buf_start, &arg, size(arg));
    return buf_start + size(arg);
  }
  static size_t unpack(char* buf, size_t buf_start, char& arg) {
    memcpy(&arg, buf+buf_start, size(arg));
    return buf_start + size(arg);
  }

  static size_t size(int32_t arg) {
    return sizeof(int32_t);
  }
  static size_t pack(char* buf, size_t buf_start, int32_t& arg) {
    int32_t to_buf = htonl(arg);
    memcpy(buf+buf_start, (char*)&to_buf, size(to_buf));
    return buf_start + size(to_buf);
  }
  static size_t unpack(char* buf, size_t buf_start, int32_t& arg) {
    int32_t from_buf;
    memcpy((char*)&from_buf, buf+buf_start, size(from_buf));
    arg = ntohl(from_buf);
    return buf_start + size(from_buf);
  }

  static size_t size(std::string& arg) {
    return sizeof(int32_t) + arg.size();
  }
  static size_t pack(char* buf, size_t buf_start, std::string& arg) {
    int32_t arg_len = arg.size();
    buf_start = pack(buf, buf_start, arg_len);
    arg.copy(buf+buf_start, arg_len);
    return buf_start + arg_len;
  }
  static size_t unpack(char* buf, size_t buf_start, std::string& arg) {
    int32_t arg_len;
    buf_start = unpack(buf, buf_start, arg_len);
    arg.assign(buf+buf_start, arg_len);
    return buf_start + arg_len;
  }
};

#endif /*_SERIALIZE_HPP*/
