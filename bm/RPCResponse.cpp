#include <cereal/types/string.hpp>
#include <cereal/archives/binary.hpp>
#include <sstream>
#include "RPCResponse.hpp"

std::string RPCResponse::debug_str() const {
    std::stringstream ss;
    ss << "\n{\n    Code: " << Code << ",\n    Body: <<" << Body << ">>,\n}\n";
    return ss.str();
}

std::string RPCResponse::packet() const {
    std::stringstream ss;
    cereal::BinaryOutputArchive archive(ss);
    archive(Code, Body);
    return ss.str();
}

RPCResponse RPCResponse::load_packet(const std::string &packet) {
    ServerCode _Code;
    std::string _Body;

    std::stringstream ss; ss.str(packet);
    cereal::BinaryInputArchive extract(ss);
    extract(_Code, _Body);

    RPCResponse request;
    request.Code = _Code;
    request.Body = _Body;

    return request;
}
