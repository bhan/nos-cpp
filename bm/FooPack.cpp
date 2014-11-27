#include <iostream>
#include "FooPack.hpp"

Function<double(Foo*)> bar_method_delegate(const std::string &str, bool t) {
    return [=](Foo *obj) { return obj->bar_method(str, t); };
}

double Foo::bar_method(const std::string &str, bool t) {
    double ret = dist(mt);
    std::cout << "Method was passed in '" << str << "' with bool " << (t ? "true" : "false") << "; returning " << ret << "\n";
    return ret;
}

FooClientProxy::FooClientProxy(const std::string &address, uint32_t port) : NOSClient(address, port) {
    _id = create();

}

FooClientProxy::~FooClientProxy() {
    del(_id);
}

uint32_t FooClientProxy::getID() {
    return _id;
}

void FooClientProxy::set_baz(int64_t val) {
    // sends RPC
}

int64_t FooClientProxy::get_baz() {
    // sends RPC
    return 42;
}

double FooClientProxy::bar_method(const std::string &str, bool t) {
    RPCRequest req;
    req.ObjectID = 42;
    req.Body = bar_method_delegate(str, t).Serialize();

    RPCResponse resp = rpc_send(req);
    return 5.0;
}

FooClientProxy FooClientProxy::get(uint32_t oid) {
    // sends RPC to get object, and returns object id
    return FooClientProxy("127.0.0.1", 1024);
}


uint32_t FooClientProxy::create() {
    // sends RPC to create object, and returns object id
    return 42;
}

void FooClientProxy::del(uint32_t oid) {
    // sends PRC over to delete object
}


uint32_t FooServerProxy::create() {
    return 42;
}

Foo FooServerProxy::get(uint32_t object_id) {
    return Foo();
}

void FooServerProxy::del(uint32_t object_id) {

}

RPCResponse FooServerProxy::process_request(const RPCRequest &request) {
    return RPCResponse();
}

FooServerProxy::FooServerProxy(uint32_t port) : NOSDaemon(port) { }

void FooServerProxy::handleRequest() {
    Foo foo;
    TCPStream* stream = TCPAcceptor::accept();
    if (stream == NULL) return;

    RPCRequest request = rpc_receive(stream);

    auto fun = Function<double(Foo*)>::Load(request.Body);
    std::cerr << "FUNC LOADED\n";
    fun(&foo);

    RPCResponse response;
    response.Code = ServerCode::OK;
    response.Body = "NO PROBLEMS";

    send_packet(stream, response.packet());
    delete stream;
}

