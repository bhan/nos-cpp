#pragma once

struct RPCRequest {
    uint32_t UUID, Type, MethodID;
    std::string ClassID, ObjectID, Body;

public:
    std::string packet() const;
    std::string to_str() const;
    static RPCRequest load_packet(const std::string &packet);
};
