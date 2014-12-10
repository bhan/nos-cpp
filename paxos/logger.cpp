#include <vector>
#include "PaxosNode.hpp"

int main() {
  std::vector<std::pair<std::string, int>> addresses;
  std::string address = "localhost"; int port = 5555;
  addresses.push_back(std::make_pair(address, port));
  PaxosNode(addresses, 0);
}
