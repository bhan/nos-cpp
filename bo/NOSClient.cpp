#include <iostream>
#include "Codes.hpp"
#include "NOSClient.hpp"

bool NOSClient::send(const std::string &buffer, std::string &responseBuffer,
                     std::string& address, uint32_t port) {
  TCPStream *stream;
  if ((stream = TCPConnector::connect(address.c_str(), port, _timeoutSeconds)) == NULL) {
    return false;
  }

  send_packet(stream, buffer);
  receive_packet(stream, responseBuffer);

  delete stream;
  return true;
}

RPCResponse NOSClient::rpc_send(const RPCRequest &request, std::string& address,
                                uint32_t port) {
  if (_debugMode) {
    std::cerr << "Request:" << request.to_str() << "\n";
  }

  std::string responseBuffer;
  if (not send(request.packet(), responseBuffer, address, port)) {
    return RPCResponse();
  }

  RPCResponse response = RPCResponse::load_packet(responseBuffer);
  if (_debugMode) {
    std::cerr << "Response:" << response.to_str() << "\n";
  }

  return response;
}

ClientObj* NOSClient::Import(std::string name, std::string& address, uint32_t port) {
  RPCRequest request;
  request.Type = static_cast<uint32_t>(MessageCode::get_type);
  request.ObjectID = name;
  RPCResponse response = _instance->rpc_send(request, address, port);

  if (response.Code != ServerCode::OK) {
    std::cout << "NOSClient::Import of " << name << " failed" << std::endl;
    return nullptr;
  }
  std::string& type = response.Body;
  std::cout << "NOSAgent::Import " << name << " of type " << type << std::endl;
  return _type_util.getClientObjFromAgentName(type, name, address, port);
}
