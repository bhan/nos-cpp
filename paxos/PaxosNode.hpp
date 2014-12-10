#ifndef _PAXOSNODE_HPP
#define _PAXOSNODE_HPP

#include <chrono>
#include <list>
#include <string>
#include <thread>
#include <vector>

#include "AcceptorGenerated.hpp"

NOSAgent* NOSAgent::_instance = NULL;
NOSClient* NOSClient::_instance = nullptr;

class PaxosNode {
 public:
  // acceptor_addresses: address:port of every acceptor, including this one
  // idx: index of acceptor_addresses for acceptor created at this node
  PaxosNode(std::vector<std::pair<std::string, int>> addresses,
            uint idx) {
    const std::string name = "Acceptor";
    auto this_address = addresses[idx].first;
    auto this_port = addresses[idx].second;
    NOSAgent::Instance(false)->initialize(this_address, this_port, 5);
    this_acceptor = new AcceptorServer();
    NOSAgent::Instance()->Export(name, this_acceptor);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    int cur_idx = 0;
    NOSClient::Instance(false)->initialize(5);
    for (auto it = addresses.begin(); it != addresses.end(); ++it, ++cur_idx) {
      if (cur_idx == idx) {
        continue;
      }
      AcceptorClient* acceptor = NULL;
      while (!acceptor) {
        acceptor = NOSClient::Instance()->Import<AcceptorClient>(name,
                                                                 it->first,
                                                                 it->second);
      }
      acceptors.push_front(acceptor);
    }

    std::cout << "ALL Acceptors imported, PaxosNode ready" << std::endl;
  }
 private:
  AcceptorServer* this_acceptor;
  std::list<AcceptorClient*> acceptors;
};

#endif /*_PAXOSNODE_HPP*/
