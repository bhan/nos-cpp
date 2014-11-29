#include <cereal/types/string.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <sstream>
#include "RPCRequest.hpp"

std::string RPCRequest::debug_str() const {
    std::stringstream ss;
    ss << "\n{\n    UUID: " << UUID << ",\n    Type: " << Type << ",\n    ClassID: " << ClassID
       << ",\n    ObjectID: " << ObjectID << ",\n    MethodID: " << MethodID << ",\n    Body: <<" << Body << ">>,\n}\n";
    return ss.str();
}

std::string RPCRequest::packet() const {
    std::stringstream ss;
    cereal::PortableBinaryOutputArchive archive(ss);
    archive(UUID, Type, ClassID, ObjectID, MethodID, Body);
    return ss.str();
}

RPCRequest RPCRequest::load_packet(const std::string &packet) {
    uint32_t _UUID, _MethodID;
    RequestType _Type;
    std::string _ClassID, _ObjectID, _Body;

    std::stringstream ss; ss.str(packet);
    cereal::PortableBinaryInputArchive extract(ss);
    extract(_UUID, _Type, _ClassID, _ObjectID, _MethodID, _Body);

    RPCRequest request;
    request.UUID = _UUID;
    request.Type = _Type;
    request.ClassID = _ClassID;
    request.ObjectID = _ObjectID;
    request.MethodID = _MethodID;
    request.Body = _Body;

    return request;
}
