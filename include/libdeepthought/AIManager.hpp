#ifndef AIMANAGER_HPP
#define AIMANAGER_HPP

#include <memory>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

class AIManager : private boost::noncopyable
{
public:
  AIManager();

  void Run();
private:
  class AIManagerConnection : public boost::enable_shared_from_this<AIManagerConnection>
  {
  public:
    typedef std::shared_ptr<AIManagerConnection> Pointer;

    static Pointer Create(boost::asio::io_service& ioService);
    boost::asio::ip::tcp::socket& Socket();
    void Start();
  private:
    AIManagerConnection(boost::asio::io_service& ioService);
    void handleWrite(const boost::system::error_code& error, size_t bytesTransferred);

    boost::asio::ip::tcp::socket socket;
  };
  
  void startAccept();
  void handleAccept(AIManagerConnection::Pointer newConnection, const boost::system::error_code& error);
  
  boost::asio::io_service service;
  boost::asio::ip::tcp::acceptor tcpAcceptor;
};

#endif
