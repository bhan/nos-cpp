#include <iostream>
#include "FooProxy.hpp"


FooProxy::FooProxy(int32_t x) : _x(x) {

}

double FooProxy::bar_method(const std::string &str, bool t) {
	double ret = dist(mt);
	std::cout << "Method was passed in '" << str << "' with bool " << (t ? "true" : "false") << "; returning " << ret << "\n";
	return ret;
}
