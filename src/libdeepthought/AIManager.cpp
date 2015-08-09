#include "libdeepthought/AIManager.hpp"

#include <boost/bind.hpp>
#include <log4cxx/logger.h>

using boost::asio::ip::tcp;

using namespace boost;
using namespace boost::asio;
using namespace log4cxx;

AIManager::AIManager()
  : service(), tcpAcceptor(service, tcp::endpoint(tcp::v4(), 1300))
{
}

void AIManager::Run()
{
  service.run();
}

void AIManager::startAccept()
{
  AIManagerConnection::Pointer newConnection = AIManagerConnection::Create(service);
  tcpAcceptor.async_accept(newConnection->Socket(), bind(&AIManager::handleAccept, this, newConnection, placeholders::error));
}

void AIManager::handleAccept(AIManagerConnection::Pointer newConnection, const boost::system::error_code& error)
{
  if (!error)
    newConnection->Start();

  startAccept();
}

AIManager::AIManagerConnection::Pointer AIManager::AIManagerConnection::Create(io_service& ioService)
{
  return AIManagerConnection::Pointer(new AIManagerConnection(ioService));
}

tcp::socket& AIManager::AIManagerConnection::Socket()
{
  return socket;
}

void AIManager::AIManagerConnection::Start()
{
  async_write(socket, buffer("asdf"), bind(&AIManagerConnection::handleWrite, shared_from_this(), placeholders::error, placeholders::bytes_transferred));
}

AIManager::AIManagerConnection::AIManagerConnection(io_service& ioService)
  : socket(ioService)
{
}
