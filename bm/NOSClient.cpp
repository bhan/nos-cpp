#include <array>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "NOSClient.hpp"

NOSClient::NOSClient(const std::string &address, uint32_t port, bool debugMode) : TCPConnector(), _address(address), _port(port), _debugMode(debugMode) {
}

NOSClient::NOSClient(const std::string &address, uint32_t port) : NOSClient(address, port, true) {
}

bool NOSClient::send(const std::string &buffer) {
    TCPStream *stream;
    if ((stream = TCPConnector::connect(_address.c_str(), _port)) == NULL) {
        return false;
    }

    stream->send(buffer.c_str(), buffer.size());
    std::cout << "sent - " << buffer.c_str() << "\n";

    string returnBuffer;
    char raw_return_buffer[256];
    int return_buffer_len = stream->receive(raw_return_buffer, sizeof(raw_return_buffer));
    returnBuffer.append(raw_return_buffer, return_buffer_len);

    std::cout << "received - " << returnBuffer << std::endl;
    delete stream;
    return true;
}

/*
int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "usage: " << argv[0] << " <ip> <port>" << std::endl;
        exit(1);
    }

    NOSClient *client = new NOSClient(argv[1], atoi(argv[2]));

    std::array<string, 2> messages = {"Message1", "Message2"};
    for (const auto &msg : messages) {
        client->send(msg);
    }

    exit(0);
}
*/