#include <iostream>
#include "tcpsockets/tcpacceptor.h"
#include "NOSCommon.hpp"

class NOSDaemon : private NOSCommon, private TCPAcceptor {
public:
    NOSDaemon(uint32_t port, bool debugMode);
    NOSDaemon(uint32_t port);

    bool start();
    void handleConnection();

private:
    uint32_t _port;
    bool _debugMode;
};
