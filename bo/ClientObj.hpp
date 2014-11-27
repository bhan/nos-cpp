#ifndef _CLIENT_OBJ_H
#define _CLIENT_OBJ_H

#include <string>

class ClientObj {
 public:
  virtual ~ClientObj() {}
  std::string name;
  std::string ip_addr;
  int port;
};
#endif /* _CLIENT_OBJ_H */
