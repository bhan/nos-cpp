#pragma once
#include "tcpsockets/tcpacceptor.h"

class NOSCommon {
public:
    NOSCommon() {}
    uint32_t send_packet(TCPStream *stream, const std::string &buffer);
    void receive_packet(TCPStream *stream, std::string &dest_buffer);

protected:
    uint32_t TCP_BUFFER_SIZE = 256;
    bool _debugMode = false;

    std::string timestamp();

private:
    const static std::string NOS_PACKET_SIGNATURE;

    std::string uint32_to_byte_array(uint32_t n);
    uint32_t byte_array_to_uint32(const std::string &byte_array);
    void send_header(TCPStream *stream, const std::string &buffer);
    uint32_t receive_header(TCPStream *stream);
    void dump_tcp_trace(const std::string &buffer, bool isSending, bool nohex=false);
};
