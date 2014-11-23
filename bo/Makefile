CPP=clang++
ARGS=-std=c++11
LD=-lpthread

agent:
	$(CPP) $(ARGS) Agent.hpp

classes:
	$(CPP) $(ARGS) BaseA.hpp BaseAGenerated.hpp BaseB.hpp BaseBGenerated.hpp

test: agent classes utils
	$(CPP) $(ARGS) -o test test.cpp $(LD)

utils:
	$(CPP) $(ARGS) TypeUtil.hpp

clean:
	rm test *.gch
