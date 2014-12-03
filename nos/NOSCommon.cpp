#include <iostream>
#include <iomanip>
#include <ctime>
#include "NOSCommon.hpp"

const std::string NOSCommon::NOS_PACKET_SIGNATURE = "NOSPKT";

std::string NOSCommon::uint32_to_byte_array(uint32_t n) {
    return std::string((char*)&n, sizeof(uint32_t));
}

uint32_t NOSCommon::byte_array_to_uint32(const std::string &byte_array) {
    return *(uint32_t*)((void*)((char*)byte_array.c_str()));
}

std::string NOSCommon::timestamp() {
    double x = (double)(time(NULL));
    time_t rawtime = time_t(x);
    struct tm *ptm  = localtime( &rawtime );

    int bufsize = 100;
    char buff[bufsize];
    strftime(buff, bufsize, "%Y-%m-%d %H:%M:%S%z", ptm);
    return string(buff);
}

void NOSCommon::send_header(TCPStream *stream, const std::string &buffer) {
    std::string header = NOS_PACKET_SIGNATURE + uint32_to_byte_array(buffer.size());
    stream->send(header.c_str(), header.size());
    if (_debugMode) {
        dump_tcp_trace(header, true);
    }
}

uint32_t NOSCommon::receive_header(TCPStream *stream) {
    std::string header; header.resize(NOS_PACKET_SIGNATURE.size() + sizeof(uint32_t));
    size_t header_len = 0;
    if ((header_len = stream->receive((char*)header.c_str(), header.size())) < header.size()) {
        throw new exception();
    }
    if (_debugMode) {
        dump_tcp_trace(header, false);
    }
    return byte_array_to_uint32( header.substr(NOS_PACKET_SIGNATURE.size()) );
}

uint32_t NOSCommon::send_packet(TCPStream *stream, const std::string &buffer) {
    // Send header info (size of the packet coming down the pipe)
    send_header(stream, buffer);

    // Send the actual data (in chunks of set size)
    uint32_t num_packets_sent = 0;
    for (uint32_t current_ptr=0; current_ptr < buffer.size(); current_ptr += TCP_BUFFER_SIZE, num_packets_sent++) {
        uint32_t remaining_size = buffer.size() - current_ptr;
        stream->send(&buffer.c_str()[current_ptr], remaining_size >= TCP_BUFFER_SIZE ? TCP_BUFFER_SIZE : remaining_size);
    }

    if (_debugMode) {
        dump_tcp_trace(buffer, true);
    }

    // return the number of chunks sent
    return num_packets_sent;
}

void NOSCommon::receive_packet(TCPStream *stream, std::string &dest_buffer) {
    // Read the header first; this will tell us the size of the packet coming down the pipe
    uint32_t expected_buffer_len = receive_header(stream);

    // Resize buffer to the expected application packet length
    dest_buffer.resize(expected_buffer_len);

    // Copy from TCP stream over to the underlying C-string
    ssize_t buffer_len;
    uint32_t current_len=0;
    while (current_len < expected_buffer_len && (buffer_len = stream->receive( &((char*)dest_buffer.c_str())[current_len], TCP_BUFFER_SIZE )) > 0) {
        current_len += buffer_len;
    }

    // Resize if necessary
    dest_buffer.resize(current_len);

    if (_debugMode) {
        dump_tcp_trace(dest_buffer, false);
    }
}

void NOSCommon::dump_tcp_trace(const std::string &buffer, bool isSending, bool nohex) {
    unsigned char *ptr = (unsigned char *)buffer.c_str();
    size_t size = buffer.size();
    unsigned int width = nohex ? 0x40 : 0x10; // without the hex output, we can fit more on screen

    std::cerr << "[" << timestamp() << "] " << (isSending ? "SENT" : "RECEIVED") << " TCP PACKET:\n";
    for (size_t i=0; i < size; i += width) {
        std::cerr  << setw(4) << setfill('0') << hex << i << ":  ";

        if (not nohex) {
            for (size_t k = 0; k < width; k++) {
                if (i+k < size) std::cerr << setw(2) << hex << (unsigned int)(unsigned char)ptr[i+k] << " ";
                else std::cerr << "   ";
            }
        }

        for (size_t k = 0; (k < width) && (i+k < size); k++) {
            // check for 0D0A; if found, skip past and start a new line of output
            if (nohex && (i+k+1 < size) && ptr[i+k]==0x0D && ptr[i+k+1]==0x0A) {
                i+=(k+2-width);
                break;
            }

            std::cerr << (((ptr[i+k]>=0x20) && (ptr[i+k]<0x80)) ? (char)ptr[i+k] : '.');

            // check again for 0D0A, to avoid an extra \n if it's at width
            if (nohex && (i+k+2 < size) && ptr[i+k+1]==0x0D && ptr[i+k+2]==0x0A) {
                i+=(k+3-width);
                break;
            }
        } std::cerr << "\n";
    }  std::cerr << "\n";
}
