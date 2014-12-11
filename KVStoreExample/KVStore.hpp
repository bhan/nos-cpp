#ifndef _KVSTORE_H
#define _KVSTORE_H

#include <mutex>
#include <string>
#include <unordered_map>

#include "../nos/NetObj.hpp"

typedef std::unordered_map<std::string, std::string> KVList;

class KVStore : public NetObj {
  public:
    KVStore() {}
    virtual ~KVStore() {}

    bool createdb(std::string dbname) {
      if (dbname.empty()) { return false; }

      _mtx.lock();

      if (_dbs.find(dbname) != _dbs.end()) { _mtx.unlock(); return false; }

      _dbs[dbname];

      _mtx.unlock(); return true;
    }

    bool removedb(std::string dbname) {
      if (dbname.empty()) { return false; }

      _mtx.lock();

      _dbs.erase(dbname);

      _mtx.unlock(); return true;
    }

    bool put(std::string dbname, std::string key, std::string value) {
      if (dbname.empty() || key.empty() || value.empty()) { return false; }

      _mtx.lock();

      auto db = _dbs.find(dbname);
      if (db == _dbs.end()) { _mtx.unlock(); return false; }

      (db->second)[key] = value;

      _mtx.unlock(); return true;
    }

    std::string get(std::string dbname, std::string key) {
      if (dbname.empty() || key.empty()) { return ""; }

      _mtx.lock();

      auto db = _dbs.find(dbname);
      if (db == _dbs.end()) { _mtx.unlock(); return ""; }

      auto got = (db->second).find(key);
      auto value = (got == (db->second).end()) ? "" : got->second;

      _mtx.unlock(); return value;
    }

    bool del(std::string dbname, std::string key) {
      if (dbname.empty() || key.empty()) { return false; }

      _mtx.lock();

      auto db = _dbs.find(dbname);
      if (db == _dbs.end()) { _mtx.unlock(); return false; }

      auto found = (db->second).erase(key);

      _mtx.unlock(); return found > 0;
    }

    KVList list(std::string dbname) {
      _mtx.lock();

      auto got = _dbs.find(dbname);
      auto res = (got == _dbs.end()) ? KVList() : got->second;

      _mtx.unlock(); return res;
    }

  private:
    std::mutex _mtx;
    std::unordered_map<std::string, KVList> _dbs;
};

#endif /* _KVSTORE_H */
