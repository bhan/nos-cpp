#include "tcpsockets/tcpconnector.h"
#include "NOSCommon.hpp"

class NOSClient : private NOSCommon, private TCPConnector {
public:
    NOSClient(const std::string &address, uint32_t port, bool debugMode);
    NOSClient(const std::string &address, uint32_t port);

    bool send(const std::string &buffer);

private:
	std::string _address;
    uint32_t _port;
    bool _debugMode;
};
