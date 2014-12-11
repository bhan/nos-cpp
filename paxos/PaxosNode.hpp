#ifndef _PAXOSNODE_HPP
#define _PAXOSNODE_HPP

#include <chrono>
#include <exception>
#include <list>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "AcceptorGenerated.hpp"

NOSAgent* NOSAgent::_instance = nullptr;
NOSClient* NOSClient::_instance = nullptr;

class PaxosNode {
 public:
  // acceptor_addresses: address:port of every acceptor, including this one
  // idx: index of acceptor_addresses for acceptor created at this node
  PaxosNode(std::vector<std::pair<std::string, int>> addresses, uint idx)
    : _this_acceptor(new AcceptorServer()), _quorum_size(addresses.size()/2 + 1),
      _cur_proposal_num(idx), _group_size(addresses.size()) {
    const std::string name = "Acceptor";
    auto this_address = addresses[idx].first;
    auto this_port = addresses[idx].second;
    NOSAgent::Instance()->initialize(this_address, this_port, 5);
    NOSAgent::Instance()->Export(name, _this_acceptor);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "initializing instance" << std::endl;
    NOSClient::Instance()->initialize(5);
    std::cout << "initiailzed instance" << std::endl;
    int cur_idx = 0;
    for (auto it = addresses.begin(); it != addresses.end(); ++it, ++cur_idx) {
      if (cur_idx == idx) { // skip the local acceptor
        continue;
      }
      AcceptorClient* acceptor = NULL;
      while (!acceptor) {
        acceptor = NOSClient::Instance()->Import<AcceptorClient>(
            name, it->first, it->second);
        if (!acceptor)
          std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      _acceptors.push_front(acceptor);
    }

    std::cout << "ALL Acceptors imported, PaxosNode ready" << std::endl;
  }

  // NOT THREAD SAFE
  // precondition: !message.empty()
  bool propose(std::string message) {
    if (message.empty()) throw std::logic_error("message must be non-empty");

    _cur_proposal_num += _group_size; // unique to proposers, new each round

    std::cout << "PREPARE n=" << _cur_proposal_num << std::endl;
    // TODO: log here
    auto prepare_res = _this_acceptor->prepare(_cur_proposal_num);
    int success_num = 1; // assume locally successful
    proposal_type proposal = { 0, message };
    for (auto it = _acceptors.begin(); it != _acceptors.end(); ++it) {
      proposal_type response;
      try {
        response = (*it)->prepare(_cur_proposal_num);
      } catch (exception& e) {
        std::cout << e.what() << std::endl;
        continue;
      }
      if (std::get<0>(response) > std::get<0>(proposal)) {
        proposal = response;
      }
      ++success_num;
    }
    if (success_num < _quorum_size) {
      std::cout << "PREPARE FAIL n=" << _cur_proposal_num << std::endl;
      return false;
    }
    std::get<0>(proposal) = _cur_proposal_num;

    std::cout << "PROPOSE n=" << _cur_proposal_num
              << " v=" << std::get<1>(proposal) << std::endl;
    // TODO: log here
    _this_acceptor->propose(proposal);
    success_num = 1;
    for (auto it = _acceptors.begin(); it != _acceptors.end(); ++it) {
      bool result;
      try {
        result = (*it)->propose(proposal);
      } catch (exception& e) {
        std::cout << e.what() << std::endl;
        continue;
      }
      if (result) { ++success_num; }
    }
    if (success_num < _quorum_size) {
      std::cout << "PROPOSE FAIL n=" << _cur_proposal_num << std::endl;
      return false;
    }

    std::cout << "DECIDED n=" << _cur_proposal_num
              << " v=" << std::get<1>(proposal) << std::endl;
    // BEGIN DECIDE PHASE
    return true;
  }
 private:
  AcceptorServer* _this_acceptor;
  std::list<AcceptorClient*> _acceptors;
  int _group_size;
  int _quorum_size;
  int _cur_proposal_num;
};

#endif /*_PAXOSNODE_HPP*/
