#pragma once

enum RequestType { IMPORT, INVOKE, RENEW_LEASE, };

struct RPCRequest {
    uint32_t UUID, ClassID, ObjectID, MethodID;
    RequestType Type;
    std::string Body;

public:
    std::string packet() const;
    std::string debug_str() const;
    static RPCRequest load_packet(const std::string &packet);
};
