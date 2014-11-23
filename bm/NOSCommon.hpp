#include "tcpsockets/tcpacceptor.h"

class NOSCommon {
public:
    NOSCommon() {}

    static std::string timestamp();
    static void dump_tcp_trace(const std::string &buffer, bool nohex=true);
    static void stream_send(TCPStream *stream, const std::string &buffer);
};