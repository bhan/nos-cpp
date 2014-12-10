#include <cstdlib>
#include <iostream>
#include <vector>

#include "PaxosNode.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "usage: ./logger <idx>" << std::endl;
    std::exit(1);
  }
  std::vector<std::pair<std::string, int>> addresses = {
    {"localhost", 5550}, {"localhost", 5551}, {"localhost", 5552}
  };

  PaxosNode(addresses, std::atoi(argv[1]));

  for (std::string line; std::getline(std::cin, line); ) {
    std::cout << "starting paxos run" << std::endl;
  }
}
