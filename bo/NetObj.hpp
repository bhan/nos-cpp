// WE WRITE THIS
#ifndef _NET_OBJ_H
#define _NET_OBJ_H

class NetObj {
 public:
  virtual ~NetObj() {}
  virtual void dispatch(char* buf, char*& res_buf, uint32_t& res_buf_size) {}
};

#endif /* _NET_OBJ_H */
