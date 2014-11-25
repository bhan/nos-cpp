#include "tcpsockets/tcpconnector.h"
#include "NOSCommon.hpp"
#include "RPCRequest.hpp"
#include "RPCResponse.hpp"

class NOSClient : private NOSCommon, private TCPConnector {
public:
    NOSClient(const std::string &address, uint32_t port, uint32_t timeout=10, bool debugMode=true);

    std::string address();
    uint32_t port();

    bool send(const std::string &buffer, std::string &responseBuffer);
    RPCResponse rpc_send(const RPCRequest &request);

private:
	std::string _address;
    uint32_t _port, _timeoutSeconds;
    bool _debugMode;
};
