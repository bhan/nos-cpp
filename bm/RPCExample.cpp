#include <iostream>
#include "RPCRequest.hpp"
#include "RPCResponse.hpp"

int main() {
    std::string packet;
    {
        RPCRequest request;
        request.UUID = 7743;
        request.ClassID = 3333;
        request.ClassID = 42;
        packet = request.packet();
        std::cout << request.to_str() << "\nsize is: " << request.packet().size() << std::endl;
    }
    {
        RPCRequest request = RPCRequest::load_packet(packet);
        std::cout << request.to_str() << std::endl;
    }

    {
        RPCResponse response;
        response.Code = ServerCode::OBJECT_NOT_FOUND;
        packet = response.packet();
        std::cout << response.to_str() << "\nsize is: " << response.packet().size() << std::endl;
    }
    {
        RPCResponse response = RPCResponse::load_packet(packet);
        std::cout << response.to_str() << std::endl;
    }
}
