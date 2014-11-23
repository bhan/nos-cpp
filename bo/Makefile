CPP=clang++
ARGS=-std=c++11

agent:
	$(CPP) $(ARGS) Agent.hpp
test: classes utils
	$(CPP) $(ARGS) -o test test.cpp

classes:
	$(CPP) $(ARGS) BaseA.hpp BaseAGenerated.hpp BaseB.hpp BaseBGenerated.hpp

utils:
	$(CPP) $(ARGS) TypeUtil.hpp

clean:
	rm test *.gch
