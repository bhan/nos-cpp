#include <iostream>
#include "SerializableClosure.hpp"
#include "FooPack.hpp"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "usage: " << argv[0] << " <ip> <port>" << std::endl;
        exit(1);
    }

    FooClientProxy *client = new FooClientProxy(argv[1], atoi(argv[2]));
    client->bar_method("example string", true);
    exit(0);
}
