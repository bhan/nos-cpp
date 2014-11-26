#include <iostream>
#include "NOSDaemon.hpp"

int main(int argc, char** argv) {
    if (argc < 2 || argc > 4) {
        std::cout << "usage: " << argv[0] << " <port> [<ip>]" << std::endl;
        exit(1);
    }

    NOSDaemon *daemon = (argc == 3) ? new NOSDaemon(atoi(argv[1]), argv[2]) : new NOSDaemon(atoi(argv[1]));
    if (not daemon->start()) { exit(1); }

    while (1) {
        daemon->handleRequest();
    }

    exit(0);
}
