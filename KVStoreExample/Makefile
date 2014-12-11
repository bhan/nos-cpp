CPP=clang++ -std=c++11
INCLUDES=-I..
LIBS=-L../tcpsockets -ltcpsockets

all: server client

server:
	$(CPP) $(INCLUDES) $(LIBS) server.cpp Registrar.cpp ../nos/NOSCommon.cpp ../nos/NOSAgent.cpp ../nos/RPCRequest.cpp ../nos/RPCResponse.cpp -o server

client:
	$(CPP) $(INCLUDES) $(LIBS) client.cpp ClientRegistrar.cpp ../nos/NOSCommon.cpp ../nos/NOSClient.cpp ../nos/RPCRequest.cpp ../nos/RPCResponse.cpp -o client


clean:
	rm -f test*.out *.gch *.o client server
