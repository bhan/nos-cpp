#include <cereal/types/string.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <sstream>
#include "RPCRequest.hpp"

std::string RPCRequest::to_str() const {
    std::stringstream ss;
    ss << "\n{\n    UUID: " << UUID << ",\n    Type: " << Type << ",\n    ClassID: " << ClassID
       << ",\n    ObjectID: " << ObjectID << ",\n    Body: <<" << Body << ">>,\n}\n";
    return ss.str();
}

std::string RPCRequest::packet() const {
    std::stringstream ss;
    cereal::PortableBinaryOutputArchive archive(ss);
    archive(UUID, Type, ClassID, ObjectID, Body);
    return ss.str();
}

RPCRequest RPCRequest::load_packet(const std::string &packet) {
    uint32_t _UUID, _ClassID, _ObjectID;
    RequestType _Type;
    std::string _Body;

    std::stringstream ss; ss.str(packet);
    cereal::PortableBinaryInputArchive extract(ss);
    extract(_UUID, _Type, _ClassID, _ObjectID, _Body);

    RPCRequest request;
    request.UUID = _UUID;
    request.Type = _Type;
    request.ClassID = _ClassID;
    request.ObjectID = _ObjectID;
    request.Body = _Body;

    return request;
}
