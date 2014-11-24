CPP=clang++
ARGS=-std=c++11
LD=-lpthread

TCP_CLASSES=../tcpsockets/tcpacceptor.cpp ../tcpsockets/tcpconnector.cpp ../tcpsockets/tcpstream.cpp


all: testserver testclient

agent:
	$(CPP) $(ARGS) Agent.hpp

classes:
	$(CPP) $(ARGS) BaseA.hpp BaseAGenerated.hpp BaseB.hpp BaseBGenerated.hpp

testserver: agent classes utils
	$(CPP) $(ARGS) -o testserver.out $(TCP_CLASSES) testserver.cpp $(LD)

testclient: agent classes utils
	$(CPP) $(ARGS) -o testclient.out $(TCP_CLASSES) testclient.cpp $(LD)

utils:
	$(CPP) $(ARGS) TypeUtil.hpp

clean:
	rm test*.out *.gch
