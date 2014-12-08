#ifndef _PAXOSNODE_H
#define _PAXOSNODE_H

#include <map>
#include <mutex>

class PaxosNode : public NetObj {
 public:
  PaxosNode() : _min_accept_num(0) {}
  virtual ~PaxosNode() {}

  std::tuple<uint32_t, std::string>
  prepare(uint32_t num) {
    std::tuple<uint32_t, std::string> response;
    _mtx.lock();
    if (num > _min_accept_num) {
      _min_accept_num = num;
      std::get<0>(response) = 0;
      std::get<1>(response) = "";
    } else {
      auto rit = _accepted.rbegin();
      std::get<0>(response) = rit->first;
      std::get<1>(response) = rit->second;
    }
    _mtx.unlock();
    return response;
  }
 private:
  std::mutex _mtx;
  uint32_t _min_accept_num;
  std::map<uint32_t, std::string> _accepted;
}

#endif /*_PAXOSNODE_H*/
