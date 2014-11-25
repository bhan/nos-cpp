#include <iostream>
#include "NOSClient.hpp"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "usage: " << argv[0] << " <ip> <port>" << std::endl;
        exit(1);
    }

    NOSClient *client = new NOSClient(argv[1], atoi(argv[2]));

    std::string msg = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$-^|&@[]}:*[{<>?,./()";
    std::string resp;
    client->send(msg, resp);
    std::cerr << "response is " << resp << "\n";

    RPCRequest request;
    RPCResponse response = client->rpc_send(request);

    exit(0);
}
