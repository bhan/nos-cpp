#include <cereal/types/string.hpp>
#include <cereal/archives/binary.hpp>
#include <sstream>
#include "RPCRequest.hpp"

std::string RPCRequest::to_str() const {
    std::stringstream ss;
    ss << "\n{\n    UUID: " << UUID << ",\n    ClassID: " << ClassID
       << ",\n    ObjectID: " << ObjectID << ",\n    Body: <<" << Body << ">>,\n}\n";
    return ss.str();
}

std::string RPCRequest::packet() const {
    std::stringstream ss;
    cereal::BinaryOutputArchive archive(ss);
    archive(UUID, ClassID, ObjectID, Body);
    return ss.str();
}

RPCRequest RPCRequest::load_packet(const std::string &packet) {
    uint32_t _UUID, _ClassID, _ObjectID;
    std::string _Body;

    std::stringstream ss; ss.str(packet);
    cereal::BinaryInputArchive extract(ss);
    extract(_UUID, _ClassID, _ObjectID, _Body);

    RPCRequest request;
    request.UUID = _UUID;
    request.ClassID = _ClassID;
    request.ObjectID = _ObjectID;
    request.Body = _Body;

    return request;
}
