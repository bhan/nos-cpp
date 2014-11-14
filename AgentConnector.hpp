#ifndef AGENT_CONNECTOR_HPP
#define AGENT_CONNECTOR_HPP

#include <string>
#include "NetObj.hpp"

class AgentConnector {
  private:
    TCPConnector connector;
    char* ip;
    int port;
  public:
    AgentConnector(char* agent_ip, int agent_port)
      : ip(agent_ip), port(agent_port), connector(TCPConnector()) {};
    void export(NetObj netObj, string name) {
    }
};

#endif // AGENT_CONNECTOR_HPP
