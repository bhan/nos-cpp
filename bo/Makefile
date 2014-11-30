CPP=clang++
ARGS=-std=c++11
LD=-lpthread

TCP_CLASSES=../tcpsockets/tcpacceptor.cpp ../tcpsockets/tcpconnector.cpp ../tcpsockets/tcpstream.cpp


all: testserver testclient

nos:
	$(CPP) $(ARGS) -I.. -c RPCRequest.cpp RPCResponse.cpp NOSCommon.cpp NOSAgent.cpp NOSClient.cpp

classes:
	$(CPP) $(ARGS) -I.. -c BaseAGenerated.hpp BaseBGenerated.hpp

testserver: nos utils
	$(CPP) $(ARGS) -I.. -o testserver.out $(TCP_CLASSES) RPCResponse.o RPCRequest.o NOSCommon.o NOSAgent.o TypeUtil.o testserver.cpp $(LD)

testclient: nos utils
	$(CPP) $(ARGS) -I.. -o testclient.out $(TCP_CLASSES) RPCResponse.o RPCRequest.o NOSCommon.o NOSClient.o TypeUtil.o testclient.cpp $(LD)

utils: classes
	$(CPP) $(ARGS) -I.. -c TypeUtil.cpp

clean:
	rm test*.out *.gch *.o
