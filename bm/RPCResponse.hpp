
enum ServerCode { OK, OBJECT_NOT_FOUND, FAIL, };

struct RPCResponse {
    ServerCode Code;
    std::string Body;

public:
    std::string packet() const;
    std::string to_str() const;
    static RPCResponse load_packet(const std::string &packet);
};
