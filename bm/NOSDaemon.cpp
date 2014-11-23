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

NOSDaemon::NOSDaemon(uint32_t port, bool debugMode) : TCPAcceptor(port), _port(port), _debugMode(debugMode) {
}

NOSDaemon::NOSDaemon(uint32_t port) : NOSDaemon(port, true) {
}

bool NOSDaemon::start() {
    std::cerr << "Starting NOS daemon [port " << _port << "]... ";
    if (TCPAcceptor::start() != 0) {
        std::cerr << "Failed to start!\n"
                  << "In Unix systems, ports in range [0, 1023] are reserved for system use,\n"
                  << "so please make sure you use sudo if you really want to use those ports!\n";
        return false;
    }
    std::cerr << "Startup successful!\n\n";
    return true;
}

void NOSDaemon::handleConnection() {
    TCPStream* stream = TCPAcceptor::accept();
    if (stream == NULL) return;

    std::string buffer;
    ssize_t buffer_len;
    char raw_buffer[1024];

    while ((buffer_len = stream->receive(raw_buffer, sizeof(raw_buffer))) > 0) {
        buffer.append(raw_buffer, buffer_len);
        if (_debugMode) {
            dump_tcp_trace(buffer);
        }

        stream_send(stream, " - OK GOT IT!");
    }

    delete stream;
}

int main(int argc, char** argv) {
    if (argc < 2 || argc > 4) {
        std::cout << "usage: " << argv[0] << " <port> [<ip>]" << std::endl;
        exit(1);
    }

    NOSDaemon *daemon = (argc == 3) ? new NOSDaemon(atoi(argv[1]), argv[2]) : new NOSDaemon(atoi(argv[1]));
    if (not daemon->start()) { exit(1); }

    while (1) {
        daemon->handleConnection();
    }

    exit(0);
}
