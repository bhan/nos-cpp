CC		= c++ -std=c++11
CFLAGS		= -g -c -Wall
LDFLAGS		= -ltcpsockets
SOURCES		= client.cpp tcpstream.cpp tcpconnector.cpp
INCLUDES	= -I..
OBJECTS		= $(SOURCES:.cpp=.o)
TARGETS		= client server

all: server client

clean:
	rm -rf $(OBJECTS) $(TARGETS)

server:
	$(CC) $(INCLUDES) $(LDFLAGS) NOSDaemon.cpp NOSCommon.cpp RPCRequest.cpp RPCResponse.cpp -o server

client:
	$(CC) $(INCLUDES) $(LDFLAGS) NOSClient.cpp NOSCommon.cpp RPCRequest.cpp RPCResponse.cpp NOSClientExample.cpp -o client

closure:
	$(CC) $(INCLUDES) $(LDFLAGS) SerializableClosureExample.cpp NOSClient.cpp NOSCommon.cpp FooProxy.cpp -o closure
