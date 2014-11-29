#include <iostream>
#include "NOSClient.hpp"

NOSClient::NOSClient(const std::string &address, uint32_t port, uint32_t timeoutSeconds, bool debugMode) :
                    TCPConnector(), _address(address), _port(port), _timeoutSeconds(timeoutSeconds), _debugMode(debugMode) {
    NOSCommon::_debugMode = false;
}

std::string NOSClient::address() {
    return _address;
}

uint32_t NOSClient::port() {
    return _port;
}

bool NOSClient::send(const std::string &buffer, std::string &responseBuffer) {
    TCPStream *stream;
    if ((stream = TCPConnector::connect(_address.c_str(), _port, _timeoutSeconds)) == NULL) {
        return false;
    }

    send_packet(stream, buffer);
    receive_packet(stream, responseBuffer);

    delete stream;
    return true;
}

RPCResponse NOSClient::rpc_send(const RPCRequest &request) {
    if (_debugMode) {
        std::cerr << "Request:" << request.debug_str() << "\n";
    }

    std::string responseBuffer;
    if (not send(request.packet(), responseBuffer)) {
        return RPCResponse();
    }

    RPCResponse response = RPCResponse::load_packet(responseBuffer);
    if (_debugMode) {
        std::cerr << "Response:" << response.debug_str() << "\n";
    }

    return response;
}
