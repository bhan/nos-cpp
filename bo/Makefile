CPP=clang++
ARGS=-std=c++11
LD=-lpthread

TCP_CLASSES=../tcpsockets/tcpacceptor.cpp ../tcpsockets/tcpconnector.cpp ../tcpsockets/tcpstream.cpp


all: testserver testclient

agent: noscommon
	$(CPP) $(ARGS) -c Agent.cpp

noscommon:
	$(CPP) $(ARGS) -c RPCRequest.cpp NOSCommon.cpp

classes:
	$(CPP) $(ARGS) -c BaseAGenerated.hpp BaseBGenerated.hpp

serialize:
	$(CPP) $(ARGS) -c Serialize.hpp

testserver: agent utils
	$(CPP) $(ARGS) -o testserver.out $(TCP_CLASSES) Agent.o TypeUtil.o testserver.cpp $(LD)

testclient: agent utils
	$(CPP) $(ARGS) -o testclient.out $(TCP_CLASSES) Agent.o TypeUtil.o testclient.cpp $(LD)

utils: classes
	$(CPP) $(ARGS) -c TypeUtil.cpp

clean:
	rm test*.out *.gch *.o
