#include "TetrinetServerPool.hpp"

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <signal.h>

using namespace std;
using namespace boost;

TetrinetServerPool::TetrinetServerPool(const std::string poolName, const unsigned poolSize, const uint32_t ipv4, const uint16_t portStart, const std::string pidFileExpr, const std::string startCmdExpr, const std::string stopCmdExpr)
  : poolName(poolName),
    poolSize(poolSize),
    ipv4(ipv4),
    portStart(portStart),
    servers()
{
  for (unsigned i = 0; i < poolSize; ++i)
    servers.push_back(new TetrinetServer(portStart + i, (format(pidFileExpr) % i).str(), (format(startCmdExpr) % i).str(), (format(stopCmdExpr) % i).str()));
}

TetrinetServerPool::~TetrinetServerPool()
{
  for (TetrinetServer* server : servers)
    delete server;
}

const unsigned TetrinetServerPool::GetServerCapacity() const
{
  return poolSize;
}

const unsigned TetrinetServerPool::GetStartedServers() const
{
  return count_if(servers.begin(), servers.end(), [](const TetrinetServer* i) { return i->Started(); });
}

const unsigned TetrinetServerPool::GetActiveServers() const
{
  return count_if(servers.begin(), servers.end(), [](const TetrinetServer* i) { return i->Started(); }); // TODO
}

const bool TetrinetServerPool::IsServerAvailable() const
{
  return GetServerCapacity() != GetActiveServers();
}

const uint32_t TetrinetServerPool::GetIPv4Address() const
{
  return ipv4;
}

void TetrinetServerPool::RunMatch(Match match)
{
}

TetrinetServerPool::TetrinetServer::TetrinetServer(const uint16_t port, const std::string pidFile, const std::string startCmd, const std::string stopCmd)
  : port(port),
    pid(-1),
    started(false),
    pidFile(pidFile),
    startCmd(startCmd),
    stopCmd(stopCmd),
    pidStream(pidFile)
{
}

TetrinetServerPool::TetrinetServer::~TetrinetServer()
{
  if (!pidStream.is_open())
    pidStream.close();
}

const uint16_t TetrinetServerPool::TetrinetServer::GetPort() const
{
  return port;
}

const bool TetrinetServerPool::TetrinetServer::Started() const
{
  return started = kill(pid, 0) == 0;
}

void TetrinetServerPool::TetrinetServer::Start()
{
  if (!pidStream.is_open())
    pidStream.close();
  system(startCmd.c_str());
  pidStream.open(pidFile);

  string pidS;
  getline(pidStream, pidS);
  pid = lexical_cast<pid_t>(pidS);
}

void TetrinetServerPool::TetrinetServer::Stop()
{
  system(stopCmd.c_str());
}

