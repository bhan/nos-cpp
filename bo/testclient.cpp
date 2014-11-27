#include <iostream>
#include <string>
#include <typeinfo>

#include "Agent.hpp"
#include "BaseA.hpp"
#include "BaseAGenerated.hpp"
#include "BaseB.hpp"
#include "BaseBGenerated.hpp"
#include "NetObj.hpp"
#include "TypeUtil.hpp"

Agent* Agent::_instance = NULL;

int main() {
  std::string ip_addr = "localhost"; int port = 5555;
  BaseBClient* baseB = dynamic_cast<BaseBClient*>(Agent::Instance()->Import("BaseB", port, ip_addr));
  baseB = dynamic_cast<BaseBClient*>(Agent::Instance()->Import("BaseB", port, ip_addr));
  std::cout << "baseB->decrement(5): " << baseB->decrement(5) << std::endl;
}
