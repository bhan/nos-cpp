#include <iostream>
#include "tcpsockets/tcpacceptor.h"

int main(int argc, char** argv) {
  if (argc < 2 || argc > 4) {
    std::cout << "usage: agent <port> [<ip>]" << std::endl;
    exit(1);
  }
  TCPAcceptor* acceptor = (argc == 3)
    ? new TCPAcceptor(atoi(argv[1]), argv[2])
    : new TCPAcceptor(atoi(argv[1]));
  if (acceptor->start() != 0) {
    std::cout << "agent failed to start" << std::endl;
    exit(1);
  }
  while (1) {
    TCPStream* stream = acceptor->accept();
    if (stream == NULL) {
      continue;
    }
    ssize_t len;
    char line[256];
    while ((len = stream->receive(line, sizeof(line))) > 0) {
      line[len] = '\0';
      std::cout << "received - " << line << std::endl;
      stream->send(line, len);
    }
    delete stream;
  }
  exit(0);
}
