#include <ctime>
#include <iostream>
#include <mutex>
#include <thread>

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
  RPCResponse response;
  if (not send(request.packet(), responseBuffer, address, port)) {
    response.Code = ServerCode::FAIL;
    return response;
  }

  response = RPCResponse::load_packet(responseBuffer);
  if (_debugMode) {
    std::cerr << "Response:" << response.to_str() << "\n";
  }

  return response;
}

void NOSClient::mark_obj_deleted(std::string objectID) {
  _mtx.lock();
  _ObjectID_to_ClientObj.erase(objectID);
//  std::cerr << objectID << "'s lease will not be renewed" << std::endl;
  _mtx.unlock();
}

static void _renew_leases(std::mutex& mtx, NOSClient* client,
    std::unordered_map<std::string, ClientObj*>& ObjectID_to_ClientObj,
    uint renew_seconds, volatile bool& should_exit) { // TODO: batching
  mtx.lock();
  while (!should_exit) {
    mtx.unlock();

    mtx.lock();
    time_t cur_time = time(NULL);
//    std::cerr << "renew_leases at " << cur_time << std::endl;
    RPCRequest request;
    request.Type = static_cast<uint32_t>(RequestType::renew_lease);
    for (auto it = ObjectID_to_ClientObj.begin();
         it != ObjectID_to_ClientObj.end(); ++it) {
      request.ObjectID = it->first;
      auto clientObj = it->second;
      auto response = client->rpc_send(request, clientObj->_address,
                                       clientObj->_port); //TODO add connection error to response
      if (response.Code != ServerCode::OK) {
//        std::cerr << "renew_lease at " << cur_time << " for " << it->first << " failed" << std::endl;
      } else {
//        std::cerr << "renew_lease at " << cur_time << " for " << it->first << " done" << std::endl;
      }
    }
    mtx.unlock();

    std::this_thread::sleep_for(std::chrono::seconds(renew_seconds));
    mtx.lock();
  }
}

void NOSClient::initialize(uint renew_seconds) {
  _should_exit = false;
  std::thread(_renew_leases, std::ref(_mtx), _instance,
              std::ref(_ObjectID_to_ClientObj), renew_seconds,
              std::ref(_should_exit)).detach();
}

void NOSClient::exit() {
  _mtx.lock();
  _should_exit = true;
  _mtx.unlock();
}
