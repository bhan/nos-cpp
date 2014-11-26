#include <fstream>
#include <iomanip>
#include <iostream>
#include <cstdio>
#include <cstring>

#include "SerializableClosure.hpp"
#include "NOSClient.hpp"
#include "FooProxy.hpp"

template<typename X> std::string Dump(const X& x) {
    const unsigned char *s = reinterpret_cast<const unsigned char*>(&x);
    std::string d;
    for (int i = 0; i < sizeof(x); ++i) {
        char buf[3];
        sprintf(buf, "%02x", s[i]);
        d += buf;
    }
    return d;
}

std::string Dumpstr(const std::string& s) {
    std::string d;
    for (int i = 0; i < s.size(); ++i) {
        char buf[3];
        sprintf(buf, "%02x", static_cast<unsigned char>(s[i]));
        d += buf;
    }
    return d;
}

std::string Undumpstr(const std::string& s) {
    size_t l = s.size() / 2;
    char bytes[l];
    for (int i = 0; i < l; ++i) {
        sscanf(&s.c_str()[i * 2], "%2hhx", &bytes[i]);
    }
    return std::string(bytes, l);
}

Function<int(int)> f(int i) {
    auto x = [=](int y){ return y + i; };
    return x;
}

Function<int()> g(int i) {
    auto x = [=](){ return i; };
    return x;
}

Function<bool(NOSClient*)> delegate(const std::string &buffer) {
    return [=](NOSClient *client){
        std::string response;
        return client->send(buffer, response);
    };
}

Function<double(FooProxy*)> foo_delegate(const std::string &str, bool t) {
    auto x = [=](FooProxy *obj){ return obj->bar_method(str, t); };
    return x;
}

void runInNewStack(NOSClient *client, const std::string &closure) {
    std::cout << "Serialized: " << closure << "\n";
    // std::cout << "Serialized: " << Dumpstr(closure) << "\n";
    std::cout << "Byte-size of closure: " << closure.size() << "\n";
    auto uncompressed_function = Function<bool(NOSClient*)>::Load(closure);
    uncompressed_function(client);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "usage: " << argv[0] << " <ip> <port>" << std::endl;
        exit(1);
    }

    NOSClient *client = new NOSClient(argv[1], atoi(argv[2]));

    // auto my_delegate = delegate("Hello delegate method!");

    auto my_delegate = [](const std::string &buffer) -> Function<bool(NOSClient*)> {
        return [=](NOSClient *client){
            std::string response;
            return client->send(buffer, response);
        };
    }("It's code, bra.");


    std::string my_commpressed_delegate = my_delegate.Serialize();
    std::cout << "HexDump of method call: " << my_delegate.HexDump() << "\n";
    runInNewStack(client, my_commpressed_delegate);

    std::cout << "We can call methods that take no parameters as well! - " << g(5)() << "\n";
    return 0;
}