#include "tcpsockets/tcpacceptor.h"
#include "NOSCommon.hpp"
#include "RPCRequest.hpp"
#include "RPCResponse.hpp"

class NOSDaemon : private NOSCommon, private TCPAcceptor {
public:
    NOSDaemon(uint32_t port, bool debugMode);
    NOSDaemon(uint32_t port);

    bool start();
    void handleRequest();

protected:
	RPCRequest rpc_receive(TCPStream* stream);

private:
    uint32_t _port;
    bool _debugMode;
};
