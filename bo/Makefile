CPP=clang++
ARGS=-std=c++11
LD=-lpthread
TCP_CLASSES=../tcpsockets/tcpacceptor.cpp ../tcpsockets/tcpconnector.cpp ../tcpsockets/tcpstream.cpp


agent:
	$(CPP) $(ARGS) Agent.hpp

classes:
	$(CPP) $(ARGS) BaseA.hpp BaseAGenerated.hpp BaseB.hpp BaseBGenerated.hpp

test: agent classes utils
	$(CPP) $(ARGS) -o test $(TCP_CLASSES) test.cpp $(LD)

utils:
	$(CPP) $(ARGS) TypeUtil.hpp

clean:
	rm test *.gch
