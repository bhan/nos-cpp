CPP=clang++
ARGS=-std=c++11
LD=-lpthread

TCP_CLASSES=../tcpsockets/tcpacceptor.cpp ../tcpsockets/tcpconnector.cpp ../tcpsockets/tcpstream.cpp


all: testserver testclient

agent: nos
	$(CPP) $(ARGS) -I.. -c Agent.cpp

nos:
	$(CPP) $(ARGS) -I.. -c RPCRequest.cpp RPCResponse.cpp NOSClient.cpp NOSCommon.cpp

classes:
	$(CPP) $(ARGS) -I.. -c BaseAGenerated.hpp BaseBGenerated.hpp

serialize:
	$(CPP) $(ARGS) -I.. -c Serialize.hpp

testserver: agent utils
	$(CPP) $(ARGS) -o testserver.out $(TCP_CLASSES) RPCResponse.o RPCRequest.o NOSClient.o NOSCommon.o Agent.o TypeUtil.o testserver.cpp $(LD)

testclient: agent utils
	$(CPP) $(ARGS) -o testclient.out $(TCP_CLASSES) RPCResponse.o RPCRequest.o NOSClient.o NOSCommon.o Agent.o TypeUtil.o testclient.cpp $(LD)

utils: classes
	$(CPP) $(ARGS) -c TypeUtil.cpp

clean:
	rm test*.out *.gch *.o
