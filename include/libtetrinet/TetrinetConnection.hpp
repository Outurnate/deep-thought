#ifndef TETRINETCONNECTION_HPP
#define TETRINETCONNECTION_HPP

#include <boost/optional.hpp>
#include <boost/asio/ip/address.hpp>

#include "libtetrinet/TetrinetForward.hpp"

class TetrinetConnection
{
  void AddServer(boost::asio::ip::address serverAddress, uint16_t serverPort);
  void AddClient(boost::asio::ip::address clientAddress, uint16_t clientPort);
  bool HasProtocol() const;
  void SetProtocol(ProtocolVersion version);
  bool SupportsMemoryConnection();
private:
  boost::optional<boost::asio::ip::address> client, server;
  boost::optional<uint16_t> clientPort, serverPort;
  boost::optional<ProtocolVersion> protocol;
};

#endif
