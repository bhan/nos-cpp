#include <iostream>
#include <string>
#include <typeinfo>

#include "NOSClient.hpp"
#include "NetObj.hpp"
#include "../output/BaseAGenerated.hpp"
#include "../output/BaseBGenerated.hpp"

NOSClient* NOSClient::_instance = nullptr;

int main() {
  std::string address = "localhost"; int port = 5555;

  NOSClient::Instance()->initialize(5);
  BaseAClient* baseA = NOSClient::Instance()->Import<BaseAClient>("BaseA", address, port);
  if (baseA == NULL) {
    std::cout << "could not get remote object" << std::endl;
    exit(1);
  }
  BaseBClient* baseB = NOSClient::Instance()->Import<BaseBClient>("BaseB", address, port);
  if (baseB == NULL) {
    std::cout << "could not get remote object" << std::endl;
    exit(1);
  }

  auto result1 = baseB->decrement(5);
  std::cout << "baseB->decrement(5): " << result1 << std::endl;

  auto result2 = baseB->increment(5);
  std::cout << "baseB->increment(5): " << result2 << std::endl;

  while (1) {}
}
