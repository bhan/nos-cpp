#pragma once

enum RequestType { IMPORT, INVOKE, RENEW_LEASE, };

struct RPCRequest {
    uint32_t UUID, ClassID, MethodID;
    RequestType Type;
    std::string ClassId, ObjectID, Body;

public:
    std::string packet() const;
    std::string debug_str() const;
    static RPCRequest load_packet(const std::string &packet);
};
