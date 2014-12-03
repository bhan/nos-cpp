#include <array>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "tcpconnector.h"

int main(int argc, char** argv)
{
  if (argc != 3) {
    std::cout << "usage: " << argv[0] << " <port> <ip>" << std::endl;
    exit(1);
  }
  int port = atoi(argv[1]);
  char* ip = argv[2];

  std::array<string, 2> messages = {"Is there life on Mars?", "Why is there air?"};

  TCPConnector* connector = new TCPConnector();

  for (int i = 0; i < messages.size(); ++i) {
    TCPStream* stream;
    if ((stream = connector->connect(ip, port)) == NULL) {
      continue;
    }
    string& message = messages[i];
    stream->send(message.c_str(), message.size());
    std::cout << "sent - " << message.c_str() << std::endl;
    char line[256];
    int len = stream->receive(line, sizeof(line));
    line[len] = '\0';
    std::cout << "received - " << line << std::endl;
    delete stream;
  }
  exit(0);
}
