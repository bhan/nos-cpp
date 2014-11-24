#include "tcpsockets/tcpacceptor.h"

class NOSCommon {
public:
    NOSCommon() {}

protected:
    static std::string timestamp();
    static void dump_tcp_trace(const std::string &buffer, bool nohex=false);
    static uint32_t send_packet(TCPStream *stream, const std::string &buffer);
    static void receive_packet(TCPStream *stream, std::string &dest_buffer);

private:
	const static uint32_t TCP_BUFFER_SIZE = 256;
	static std::string uint32_to_byte_array(uint32_t n);
	static uint32_t byte_array_to_uint32(const std::string &byte_array);
    static void send_header(TCPStream *stream, const std::string &buffer);
    static uint32_t receive_header(TCPStream *stream);
};
