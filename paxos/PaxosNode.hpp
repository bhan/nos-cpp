#ifndef _PAXOSNODE_HPP
#define _PAXOSNODE_HPP
#include <string>
#include <vector>
#include "AcceptorGenerated.hpp"

NOSAgent* NOSAgent::_instance = NULL;

class PaxosNode {
 public:
  // acceptor_addresses: address:port of every acceptor, including this one
  // idx: index of acceptor_addresses for acceptor created at this node
  PaxosNode(std::vector<std::pair<std::string, int>> addresses,
            uint idx) {
    auto this_address = addresses[idx].first;
    auto this_port = addresses[idx].second;
    NOSAgent::Instance()->initialize(this_address, this_port, 5);
    this_acceptor = new AcceptorServer();
    NOSAgent::Instance()->Export("Acceptor", this_acceptor);
    int cur_idx = 0;
    for (auto it = addresses.begin(); it != addresses.end(); ++it, ++cur_idx) {
    }
  }
 private:
  AcceptorServer* this_acceptor;
};

#endif /*_PAXOSNODE_HPP*/
