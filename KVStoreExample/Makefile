CPP=clang++ -std=c++11
INCLUDES=-I..
LIBS=-L../nos/tcpsockets -ltcpsockets

all: server client

tcpsockets:
	cd ../nos/tcpsockets && make -f Makefile.library && cp libtcpsockets.so.1 ../../KVStoreExample/
server: tcpsockets
	$(CPP) $(INCLUDES) $(LIBS) server.cpp Registrar.cpp ../nos/NOSCommon.cpp ../nos/NOSAgent.cpp ../nos/RPCRequest.cpp ../nos/RPCResponse.cpp -o server

client: tcpsockets
	$(CPP) $(INCLUDES) $(LIBS) client.cpp ClientRegistrar.cpp ../nos/NOSCommon.cpp ../nos/NOSClient.cpp ../nos/RPCRequest.cpp ../nos/RPCResponse.cpp -o client


clean:
	rm -f test*.out *.gch *.o client server libtcpsockets.so.1
