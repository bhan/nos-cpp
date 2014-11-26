CC			= c++ -std=c++11
CFLAGS		= -g -Wall
INCLUDES	= -I..
LINKERS		= -L. -L../tcpsockets
TARGETLIB	= libnos

all: server client

libtcpsockets:
	cd ../tcpsockets && make -f Makefile.library

$(TARGETLIB): libtcpsockets
	$(CC) $(CFLAGS) $(INCLUDES) -c RPCRequest.cpp RPCResponse.cpp NOSCommon.cpp NOSDaemon.cpp NOSClient.cpp
	$(CC) -shared -ltcpsockets -Wl,-install_name,$(TARGETLIB).so.1 -o $(TARGETLIB).so.1.0 *.o
	ln -sf $(TARGETLIB).so.1.0 $(TARGETLIB).so
	ln -sf $(TARGETLIB).so.1.0 $(TARGETLIB).so.1

server: $(TARGETLIB)
	$(CC) $(CFLAGS) $(INCLUDES) $(LINKERS) -lnos -ltcpsockets NOSDaemonExample.cpp -o server

client: $(TARGETLIB)
	$(CC) $(CFLAGS) $(INCLUDES) $(LINKERS) -lnos -ltcpsockets NOSClientExample.cpp -o client

closure: $(TARGETLIB)
	$(CC) $(CFLAGS) $(INCLUDES) $(LINKERS) -lnos -ltcpsockets SerializableClosureExample.cpp FooProxy.cpp -o closure

clean:
	rm -rf *.o *.so *.so.1 *.so.1.0 server client *.dSYM
