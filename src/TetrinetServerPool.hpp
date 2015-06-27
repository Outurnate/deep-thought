#ifndef TETRINETSERVERPOOL_HPP
#define TETRINETSERVERPOOL_HPP

#include <string>
#include <fstream>
#include <vector>
#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>
#include <boost/signals2/connection.hpp>

#include "Match.hpp"

enum class TetrinetServerState
{
  STOPPED,
  VACANT,
  OCCUPIED
};

class TetrinetServerPool : private boost::noncopyable
{
public:
  explicit TetrinetServerPool(const std::string poolName, const unsigned poolSize, const uint32_t ipv4, const uint16_t portStart, const std::string pidFileExpr, const std::string startCmdExpr, const std::string stopCmdExpr);
  ~TetrinetServerPool();

  const unsigned GetServerCapacity() const;
  const unsigned GetStartedServers() const;
  const unsigned GetActiveServers() const;
  const bool IsServerAvailable() const;
  const uint32_t GetIPv4Address() const;

  void RunMatch(Match& match);
  void StartServers();
private:
  class TetrinetServer : private boost::noncopyable
  {
  public:
    TetrinetServer(const uint16_t port, const std::string pidFile, const std::string startCmd, const std::string stopCmd);
    ~TetrinetServer();

    const uint16_t GetPort() const;
    const TetrinetServerState GetState() const;
    const boost::optional<const Match&> GetMatch() const;

    void SetMatch(const Match& match);
    void ClearMatch(const Match& match);
    void Start();
    void Stop();
  private:
    const uint16_t port;
    pid_t pid;
    mutable TetrinetServerState state;
    const std::string pidFile, startCmd, stopCmd;
    std::ifstream pidStream;
    boost::optional<const Match&> match;
    boost::optional<const boost::signals2::connection&> onCompleteConnection;
  };
  
  const std::string poolName;
  const unsigned poolSize;
  const uint32_t ipv4;
  const uint16_t portStart;
  std::vector<TetrinetServer*> servers;
};

#endif