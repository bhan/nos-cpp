CPP=clang++ -std=c++11
INCLUDES=-I..
LIBS=-L../nos/tcpsockets -ltcpsockets

all: server client

kvstore:
	-../build/bin/idlgen --clang-option="--std=c++11" -t ../idlgen/tpl/generated.tpl -o ./ -r ../idlgen/tpl/registrar.tpl -x ../idlgen/tpl/client_registrar.tpl KVStore.hpp

tcpsockets:
	cd ../nos/tcpsockets && make -f Makefile.library && cp libtcpsockets.so.1 ../../KVStoreExample/
server: tcpsockets kvstore
	$(CPP) $(INCLUDES) $(LIBS) server.cpp Registrar.cpp ../nos/NOSCommon.cpp ../nos/NOSAgent.cpp ../nos/RPCRequest.cpp ../nos/RPCResponse.cpp -o server

client: tcpsockets kvstore
	$(CPP) $(INCLUDES) $(LIBS) client.cpp ClientRegistrar.cpp ../nos/NOSCommon.cpp ../nos/NOSClient.cpp ../nos/RPCRequest.cpp ../nos/RPCResponse.cpp -o client


clean:
	rm -f test*.out *.gch *.o client server libtcpsockets.so.1
