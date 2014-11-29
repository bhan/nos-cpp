#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "NOSDaemon.hpp"

void init_daemon(void) {
    pid_t child_pid, pid = 0;
    if ( (child_pid = fork()) < 0) { // check to see if we can get a child
        perror("Fork sub-process failed !n");
        exit(1);
    } else if (child_pid > 0) {
        exit(0);
    }
    setsid();
    chdir("/tmp");
    umask(0);

    // Close down the stdin, stdout, and stderr pipes
    int fd = open("/dev/null",O_RDWR);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);
}

NOSDaemon::NOSDaemon(uint32_t port, bool debugMode) : TCPAcceptor(port), _port(port), _debugMode(debugMode) { }

bool NOSDaemon::start() {
    std::cerr << "Starting NOS daemon [port " << _port << "]... ";
    if (TCPAcceptor::start() != 0) {
        std::cerr << "Failed to start!\n"
                  << "In Unix systems, ports in range [0, 1023] are reserved for system use,\n"
                  << "so please make sure you use sudo if you want to use those ports!\n";
        return false;
    }
    std::cerr << "Startup successful!\n\n";
    return true;
}

RPCRequest NOSDaemon::rpc_receive(TCPStream* stream) {
    std::string buffer;
    receive_packet(stream, buffer);

    RPCRequest request = RPCRequest::load_packet(buffer);
    if (_debugMode) {
        std::cerr << "Request:" << request.debug_str() << "\n";
    }
    return request;
}

void NOSDaemon::handleRequest() {
    TCPStream* stream = TCPAcceptor::accept();
    if (stream == NULL) return;

    std::string buffer;
    receive_packet(stream, buffer);

    RPCResponse response;
    response.Code = ServerCode::FAIL;
    response.Body = "-> PACKET RECEIVED!";

    send_packet(stream, response.packet());
    delete stream;
}
