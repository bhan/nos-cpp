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
  NetObj* netObj = new BaseAClient();
  std::cout << typeid(*netObj).name() << " " << typeid(*netObj).hash_code() << std::endl;
  BaseA* baseA = new BaseAClient();
  std::cout << typeid(*baseA).name() << " " << typeid(*baseA).hash_code() << std::endl;
  BaseAWrapper* baseAWrapper = new BaseAWrapper(*baseA);
  std::cout << typeid(*baseAWrapper).name() << " " << typeid(*baseAWrapper).hash_code() << std::endl;
  BaseAClient* baseAClient = new BaseAClient();
  std::cout << typeid(*baseAClient).name() << " " << typeid(*baseAClient).hash_code() << std::endl;
//  baseAClient->increment();
  baseAWrapper->increment();

  NetObj* netObjB = new BaseBClient();
  std::cout << typeid(*netObjB).name() << " " << typeid(*netObjB).hash_code() << std::endl;

  TypeUtil typeUtil;
  NetObj* typeUtilA = typeUtil.getClientObjFromName(typeid(BaseA).name());
  if (typeUtilA == NULL) {
    std::cout << "typeUtilA is NULL" << std::endl;
  } else {
    std::cout << typeid(*typeUtilA).name() << " " << typeid(*typeUtilA).hash_code() << std::endl;
    NetObj& typeUtilARef = *typeUtilA;
      std::cout << typeid(typeUtilARef).name() << " " << typeid(typeUtilARef).hash_code() << std::endl;
  }
//  std::cout << typeid(*typeUtilA).name() << " " << typeid(*typeUtilA).hash_code() << std::endl;
  std::string ip_addr = "localhost"; int port = 8888;
  Agent::Instance()->Initialize(port, ip_addr);
  BaseA* helloObj = new BaseA();
  Agent::Instance()->Export("hello", helloObj);
  Agent::Instance()->Export("goodbye", helloObj);
  Agent::Instance()->PrintExported();
  Agent::Instance()->Exit();
}
