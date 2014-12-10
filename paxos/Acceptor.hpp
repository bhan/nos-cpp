#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include <map>
#include <mutex>
#include <tuple>

#include "../nos/NetObj.hpp"

typedef std::tuple<uint32_t, std::string> proposal_type;

class Acceptor : public NetObj {
 public:
  Acceptor() : _min_accept_num(0) {}
  virtual ~Acceptor() {}

  std::tuple<uint32_t, std::string> prepare(uint32_t num) {
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

  bool accept(std::tuple<uint32_t, std::string> accept_request) {
    _mtx.lock();
    uint32_t proposal_num = std::get<0>(accept_request);
    if (proposal_num < _min_accept_num) {
      return false;
    }
    std::string& message = std::get<1>(accept_request);
    _accepted[proposal_num] = message;
    return true;
  }
 private:
  std::mutex _mtx;
  uint32_t _min_accept_num;
  std::map<uint32_t, std::string> _accepted;
};

#endif /*_ACCEPTOR_H*/
