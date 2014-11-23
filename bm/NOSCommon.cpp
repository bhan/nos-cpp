#include <iostream>
#include <iomanip>
#include <ctime>
#include "NOSCommon.hpp"

std::string NOSCommon::timestamp() {
    double x = (double)(time(NULL));
    time_t rawtime = time_t(x);
    struct tm *ptm  = localtime( &rawtime );

    int bufsize = 100;
    char buff[bufsize];
    strftime(buff, bufsize, "%Y-%m-%d %H:%M:%S%z", ptm);
    return string(buff);
}

void NOSCommon::stream_send(TCPStream *stream, const std::string &buffer) {
    stream->send(buffer.c_str(), buffer.size());
}

void NOSCommon::dump_tcp_trace(const std::string &buffer, bool nohex) {
    unsigned char *ptr = (unsigned char *)buffer.c_str();
    ssize_t size = buffer.size();
    unsigned int width = nohex ? 0x40 : 0x10; // without the hex output, we can fit more on screen

    std::cerr << "[" << timestamp() << "] RECEIVED TCP PACKET:\n";
    for (ssize_t i=0; i < size; i += width) {
        std::cerr  << setw(4) << setfill('0') << hex << i << ":  ";

        if (not nohex) {
            for (ssize_t  k = 0; k < width; k++) {
                if (i+k < size) std::cerr << setw(2) << hex << (unsigned int)(unsigned char)ptr[i+k] << " ";
                else std::cerr << "   ";
            }
        }

        for (ssize_t k = 0; (k < width) && (i+k < size); k++) {
            /* check for 0D0A; if found, skip past and start a new line of output */
            if (nohex && (i+k+1 < size) && ptr[i+k]==0x0D && ptr[i+k+1]==0x0A) {
                i+=(k+2-width);
                break;
            }

            std::cerr << (((ptr[i+k]>=0x20) && (ptr[i+k]<0x80)) ? (char)ptr[i+k] : '.');

            /* check again for 0D0A, to avoid an extra \n if it's at width */
            if (nohex && (i+k+2 < size) && ptr[i+k+1]==0x0D && ptr[i+k+2]==0x0A) {
                i+=(k+3-width);
                break;
            }
        } std::cerr << "\n";
    }  std::cerr << "\n";
}
