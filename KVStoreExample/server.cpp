// c++ -std=c++11 -I.. server.cpp Registrar.cpp ../nos/NOSCommon.cpp ../nos/NOSAgent.cpp ../nos/RPCRequest.cpp ../nos/RPCResponse.cpp -L../tcpsockets -ltcpsockets -o server
#include <iostream>
#include <string>

#include "KVStoreGenerated.hpp"
#include "../netobj_classes/KVStore.hpp"

NOSAgent* NOSAgent::_instance = NULL;

int main() {
    std::string address = "localhost"; int port = 5555;
    NOSAgent::Instance()->initialize(address, port, 5);
    KVStoreServer* kvstore = new KVStoreServer();
    NOSAgent::Instance()->Export("KVStore", kvstore);
    NOSAgent::Instance()->print_exported();
    while (1) {}
    NOSAgent::Instance()->exit();
}
