#include <iostream>
#include <string>
#include <typeinfo>

#include "NOSAgent.hpp"
#include "BaseA.hpp"
#include "BaseAGenerated.hpp"
#include "BaseB.hpp"
#include "BaseBGenerated.hpp"
#include "NetObj.hpp"
#include "TypeUtil.hpp"

NOSAgent* NOSAgent::_instance = NULL;

int main() {
  std::string address = "localhost"; int port = 5555;
  NOSAgent::Instance()->initialize(address, port, 5);
  BaseAServer* baseA = new BaseAServer();
  BaseBServer* baseB = new BaseBServer(10);
  NOSAgent::Instance()->Export("BaseA", baseA);
  NOSAgent::Instance()->Export("BaseB", baseB);
  NOSAgent::Instance()->print_exported();
  delete baseA;
  delete baseB;
  while (1) {
  }
  NOSAgent::Instance()->exit();
}
