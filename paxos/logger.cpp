#include <cstdlib>
#include <iostream>
#include <vector>

#include "PaxosNode.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "usage: ./logger <idx>" << std::endl;
    std::exit(1);
  }
  std::vector<std::pair<std::string, int>> addresses;
  std::string address = "localhost"; int port = 5555;
  addresses.push_back(std::make_pair(address, port));
  PaxosNode(addresses, std::atoi(argv[1]));
}
