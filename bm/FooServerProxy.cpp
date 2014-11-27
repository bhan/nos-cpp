#include <iostream>
#include "SerializableClosure.hpp"
#include "NOSDaemon.hpp"
#include "FooPack.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " <port>" << std::endl;
        exit(1);
    }

    FooServerProxy *daemon = new FooServerProxy(atoi(argv[1]));
    if (not daemon->start()) { exit(1); }

    while (1) {
        daemon->handleRequest();
    }

    exit(0);
}
