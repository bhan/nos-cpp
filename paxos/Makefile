logger:
	c++ -std=c++11 -I.. ../nos/NOSCommon.cpp ../nos/NOSAgent.cpp ../nos/NOSClient.cpp ../nos/RPCRequest.cpp ../nos/RPCResponse.cpp Registrar.cpp ClientRegistrar.cpp ../nos/tcpsockets/tcp*.cpp logger.cpp -o logger

clean:
	rm -f logger

.PHONY: logger clean
