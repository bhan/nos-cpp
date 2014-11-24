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
  std::string name = "hello";
  NetObj* netObj = Agent::Instance()->Import(name, port, ip_addr);
  std::cout << name << " of type " << typeid(*netObj).name() << std::endl;
}
