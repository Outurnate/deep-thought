#ifndef TETRINETSERVERPOOL_HPP
#define TETRINETSERVERPOOL_HPP

#include <string>
#include <fstream>
#include <vector>

#include "Match.hpp"

class TetrinetServerPool
{
public:
  explicit TetrinetServerPool(const std::string poolName, const unsigned poolSize, const uint32_t ipv4, const uint16_t portStart, const std::string pidFileExpr, const std::string startCmdExpr, const std::string stopCmdExpr);
  ~TetrinetServerPool();

  const unsigned GetServerCapacity() const;
  const unsigned GetStartedServers() const;
  const unsigned GetActiveServers() const;
  const bool IsServerAvailable() const;
  const uint32_t GetIPv4Address() const;

  void RunMatch(Match match);
private:
  class TetrinetServer
  {
  public:
    TetrinetServer(const uint16_t port, const std::string pidFile, const std::string startCmd, const std::string stopCmd);
    ~TetrinetServer();

    const uint16_t GetPort() const;
    const bool Started() const;

    void Start();
    void Stop();
  private:
    uint16_t port;
    pid_t pid;
    mutable bool started;
    std::string pidFile, startCmd, stopCmd;
    std::ifstream pidStream;
  };
  
  const std::string poolName;
  const unsigned poolSize;
  const uint32_t ipv4;
  const uint16_t portStart;
  std::vector<TetrinetServer*> servers;
};

#endif
