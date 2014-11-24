#include <array>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "NOSClient.hpp"

NOSClient::NOSClient(const std::string &address, uint32_t port, uint32_t timeoutSeconds, bool debugMode) :
                    TCPConnector(), _address(address), _port(port), _timeoutSeconds(timeoutSeconds), _debugMode(debugMode) {
}

bool NOSClient::send(const std::string &buffer) {
    TCPStream *stream;
    if ((stream = TCPConnector::connect(_address.c_str(), _port, _timeoutSeconds)) == NULL) {
        return false;
    }

    //stream_send(stream, buffer);
    send_packet(stream, buffer);
    std::cout << "Sent - " << buffer.c_str() << "\n";

    string returnBuffer;
    receive_packet(stream, returnBuffer);

    std::cout << "Received response - " << returnBuffer << std::endl;
    delete stream;
    return true;
}



int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "usage: " << argv[0] << " <ip> <port>" << std::endl;
        exit(1);
    }

    NOSClient *client = new NOSClient(argv[1], atoi(argv[2]));

    std::string msg = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()";
    client->send(msg);

    exit(0);
}
