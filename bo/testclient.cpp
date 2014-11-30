#include <iostream>
#include <string>
#include <typeinfo>

#include "NOSClient.hpp"
#include "NetObj.hpp"
#include "BaseAGenerated.hpp"
#include "BaseBGenerated.hpp"

NOSClient* NOSClient::_instance = nullptr;

int main() {
  std::string address = "localhost"; int port = 5555;

  ClientObj* obj = NOSClient::Instance()->Import("BaseB", address, port);
  if (obj == NULL) {
    std::cout << "could not get remote object" << std::endl;
    exit(1);
  }
  BaseBClient* baseB = dynamic_cast<BaseBClient*>(obj);
//  std::cout << "baseB->decrement(5): " << baseB->decrement(5) << std::endl;
}
