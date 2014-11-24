#include "tcpsockets/tcpconnector.h"
#include "NOSCommon.hpp"

class NOSClient : private NOSCommon, private TCPConnector {
public:
    NOSClient(const std::string &address, uint32_t port, uint32_t timeout=10, bool debugMode=true);
    bool send(const std::string &buffer);

private:
	std::string _address;
    uint32_t _port, _timeoutSeconds;
    bool _debugMode;
};
