#include <iostream>
#include <typeinfo>

#include "BaseA.hpp"
#include "BaseAGenerated.hpp"
#include "BaseB.hpp"
#include "BaseBGenerated.hpp"
#include "NetObj.hpp"

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
}
