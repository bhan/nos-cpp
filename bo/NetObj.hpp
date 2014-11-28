// WE WRITE THIS
#ifndef _NET_OBJ_H
#define _NET_OBJ_H

#include <string>

class NetObj {
 public:
  virtual ~NetObj() {}
};

class ClientObj {
 public:
  virtual ~ClientObj() {}
  std::string name;
  std::string ip_addr;
  int port;
};

class AgentObj {
 public:
  virtual ~AgentObj() {}
  virtual void dispatch(char* buf, char*& res_buf, uint32_t& res_buf_size) = 0;
};

#endif /* _NET_OBJ_H */
