CPP=clang++
ARGS=-std=c++11
LD=-lpthread

TCP_CLASSES=tcpsockets/tcpacceptor.cpp tcpsockets/tcpconnector.cpp tcpsockets/tcpstream.cpp
GENERATED_FILES=output


all: testserver testclient

nos:
	$(CPP) $(ARGS) -I.. -c RPCRequest.cpp RPCResponse.cpp NOSCommon.cpp NOSAgent.cpp NOSClient.cpp

classes:
	$(CPP) $(ARGS) -I.. -c ../output/BaseAGenerated.hpp ../output/BaseBGenerated.hpp

kvstore:
	$(CPP) $(ARGS) -I.. -c ../output/KVStoreGenerated.hpp

testserver: nos utils
	$(CPP) $(ARGS) -I.. -o testserver.out $(TCP_CLASSES) RPCResponse.o RPCRequest.o NOSCommon.o NOSAgent.o Registrar.o testserver.cpp $(LD)

testclient: nos utils
	$(CPP) $(ARGS) -I.. -o testclient.out $(TCP_CLASSES) RPCResponse.o RPCRequest.o NOSCommon.o NOSClient.o ClientRegistrar.o testclient.cpp $(LD)

utils: classes
	$(CPP) $(ARGS) -I.. -c ../$(GENERATED_FILES)/Registrar.cpp ../$(GENERATED_FILES)/ClientRegistrar.cpp

clean:
	rm test*.out *.gch *.o
