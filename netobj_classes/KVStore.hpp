#ifndef _KVSTORE_H
#define _KVSTORE_H

#include <mutex>
#include <string>
#include <unordered_map>

#include "../nos/NetObj.hpp"

class KVStore : public NetObj {
 public:
  KVStore() {}
  virtual ~KVStore() {}

  bool put(std::string key, std::string value) {
    if (key.empty() || value.empty()) {
      return false;
    }
    _mtx.lock();
    _map[key] = value;
    _mtx.unlock();
    return true;
  }

  std::string get(std::string key) {
    _mtx.lock();
    auto got = _map.find(key);
    auto value = (got == _map.end()) ? "" : got->second;
    _mtx.unlock();
    return value;
  }

 private:
  std::mutex _mtx;
  std::unordered_map<std::string, std::string> _map;
};

#endif /* _KVSTORE_H */
