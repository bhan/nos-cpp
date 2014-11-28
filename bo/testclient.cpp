#include <iostream>
#include <string>
#include <typeinfo>

#include "Agent.hpp"
#include "NetObj.hpp"
#include "BaseAGenerated.hpp"
#include "BaseBGenerated.hpp"

Agent* Agent::_instance = NULL;

int main() {
  std::string ip_addr = "localhost"; int port = 5555;

  ClientObj* obj = Agent::Instance()->Import("BaseB", port, ip_addr);
  if (obj == NULL) {
    std::cout << "could not get remote object" << std::endl;
    exit(1);
  }
  BaseBClient* baseB = dynamic_cast<BaseBClient*>(obj);
  baseB = dynamic_cast<BaseBClient*>(Agent::Instance()->Import("BaseB", port, ip_addr));
  std::cout << "baseB->decrement(5): " << baseB->decrement(5) << std::endl;
}
