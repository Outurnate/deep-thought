#include "TetrinetServerPool.hpp"

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <signal.h>
#include <stdexcept>

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
  return count_if(servers.begin(), servers.end(), [](const TetrinetServer* i) { return i->GetState() != TetrinetServerState::STOPPED; });
}

const unsigned TetrinetServerPool::GetActiveServers() const
{
  return count_if(servers.begin(), servers.end(), [](const TetrinetServer* i) { return i->GetState() == TetrinetServerState::OCCUPIED; });
}

const bool TetrinetServerPool::IsServerAvailable() const
{
  return GetServerCapacity() != GetActiveServers();
}

const uint32_t TetrinetServerPool::GetIPv4Address() const
{
  return ipv4;
}

void TetrinetServerPool::RunMatch(Match& match)
{
  if (!IsServerAvailable())
    throw runtime_error("No servers free");

  (*find_if(servers.begin(), servers.end(), [](const TetrinetServer* i) { return i->GetState() == TetrinetServerState::VACANT; }))->SetMatch(match);
}

TetrinetServerPool::TetrinetServer::TetrinetServer(const uint16_t port, const std::string pidFile, const std::string startCmd, const std::string stopCmd)
  : port(port),
    pid(-1),
    state(TetrinetServerState::STOPPED),
    pidFile(pidFile),
    startCmd(startCmd),
    stopCmd(stopCmd),
    pidStream(pidFile),
    match(none),
    onCompleteConnection(none)
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

const TetrinetServerState TetrinetServerPool::TetrinetServer::GetState() const
{
  return state = (kill(pid, 0) == 0 ? (match != none ? TetrinetServerState::VACANT : TetrinetServerState::OCCUPIED) : TetrinetServerState::STOPPED);
}

const optional<Match&> TetrinetServerPool::TetrinetServer::GetMatch() const
{
  return match;
}

void TetrinetServerPool::TetrinetServer::SetMatch(const Match& match)
{
  if (state != TetrinetServerState::VACANT)
    throw runtime_error("Server not vacant or stopped");
  
  this->match = match;
  state = TetrinetServerState::OCCUPIED;
  onCompleteConnection = match.addOnComplete(Match::CompleteSignal::slot_type(bind(&TetrinetServerPool::TetrinetServer::ClearMatch, this, _1)));
}

void TetrinetServerPool::TetrinetServer::ClearMatch(const Match& match)
{
  if (!this->match && optional<const Match&>(const_cast<const Match&>(this->match.get())) != optional<const Match&>(match)) // this whole line is derp
    throw runtime_error("Wrong match event received, or no match in progress");
  
  state = TetrinetServerState::VACANT;
  this->match = none;
  
  if (!this->onCompleteConnection)
    throw runtime_error("Slot never connected");
  
  onCompleteConnection->disconnect();
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
