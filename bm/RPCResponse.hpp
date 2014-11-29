#pragma once

enum ServerCode { OK, OBJECT_NOT_FOUND, FAIL, };

struct RPCResponse {
    ServerCode Code;
    std::string Body;

public:
    std::string packet() const;
    std::string debug_str() const;
    static RPCResponse load_packet(const std::string &packet);
};
