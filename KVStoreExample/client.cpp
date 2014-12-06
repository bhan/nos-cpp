// ./build.sh ; mkdir output ;  ./build/bin/idlgen -t ./idlgen/tpl/generated.tpl -o KVStoreExample/ -r ./idlgen/tpl/registrar.tpl -x ./idlgen/tpl/client_registrar.tpl netobj_classes/KVStore.hpp
// c++ -std=c++11 -I.. client.cpp ClientRegistrar.cpp ../nos/NOSCommon.cpp ../nos/NOSClient.cpp ../nos/RPCRequest.cpp ../nos/RPCResponse.cpp -L../tcpsockets -ltcpsockets -o client
#include <iostream>
#include <string>

#include "KVStoreGenerated.hpp"
#include "../netobj_classes/KVStore.hpp"

NOSClient* NOSClient::_instance = nullptr;

int main() {
    std::string address = "localhost"; int port = 5555;

    NOSClient::Instance()->initialize(5);
    KVStoreClient* kvstore = NOSClient::Instance()->Import<KVStoreClient>("KVStore", address, port);
    if (kvstore == NULL) {
        std::cout << "could not get remote object" << std::endl;
        exit(1);
    }

    auto result1 = kvstore->put("foo", "baz");
    std::cout << "kvstore->put(\"foo\", \"baz\"): " << (result1 ? "true" : "false") << std::endl;

    auto result2 = kvstore->get("foo");
    std::cout << "kvstore->get(\"foo\"): " << result2 << std::endl;
}
