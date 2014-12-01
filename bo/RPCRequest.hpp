#pragma once


enum class RequestType : uint32_t {
  get_type,
  invoke,
  renew_lease
};

struct RPCRequest {
    uint32_t UUID, Type, MethodID;
    std::string ClassID, ObjectID, Body;

public:
    RPCRequest() {}
    RPCRequest(uint32_t Type_, std::string& ObjectID_, uint32_t MethodID_,
               std::string Body_)
      : Type(Type_), ObjectID(ObjectID_), MethodID(MethodID_), Body(Body_) {}
    std::string packet() const;
    std::string to_str() const;
    static RPCRequest load_packet(const std::string &packet);
};
