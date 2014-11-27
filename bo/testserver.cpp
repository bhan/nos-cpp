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
  Agent::Instance()->Initialize(port, ip_addr);
  BaseAServer* baseA = new BaseAServer();
  BaseBServer* baseB = new BaseBServer(10);
  Agent::Instance()->Export("BaseA", baseA);
  Agent::Instance()->Export("BaseB", baseB);
  Agent::Instance()->PrintExported();
//  Agent::Instance()->Exit();
  while (1) {
  }
}
