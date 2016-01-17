#include "libdeepthought/TetrinetServerPool.hpp"

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <algorithm>
#include <signal.h>
#include <stdexcept>

#include "libdeepthought/Match.hpp"

using namespace std;
using namespace boost;
using namespace boost::filesystem;

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

unsigned TetrinetServerPool::GetServerCapacity() const
{
  return poolSize;
}

unsigned TetrinetServerPool::GetStartedServers() const
{
  return count_if(servers.begin(), servers.end(), [](const TetrinetServer* i) { return i->GetState() != TetrinetServerState::STOPPED; });
}

unsigned TetrinetServerPool::GetActiveServers() const
{
  return count_if(servers.begin(), servers.end(), [](const TetrinetServer* i) { return i->GetState() == TetrinetServerState::OCCUPIED; });
}

bool TetrinetServerPool::IsServerAvailable() const
{
  return GetServerCapacity() != GetActiveServers();
}

uint32_t TetrinetServerPool::GetIPv4Address() const
{
  return ipv4;
}

void TetrinetServerPool::RunMatch(Match& match)
{
  if (!IsServerAvailable())
    throw runtime_error("No servers free");

  (*find_if(servers.begin(), servers.end(), [](const TetrinetServer* i) { return i->GetState() == TetrinetServerState::VACANT; }))->SetMatch(match);
}

void TetrinetServerPool::StartServers()
{
  for (TetrinetServer* server : servers)
    server->Start();
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
  if (GetState() != TetrinetServerState::STOPPED)
    Stop();
}

uint16_t TetrinetServerPool::TetrinetServer::GetPort() const
{
  return port;
}

TetrinetServerState TetrinetServerPool::TetrinetServer::GetState() const
{
  if (!exists(pidFile))
    return TetrinetServerState::STOPPED;
  return state = (kill(pid, 0) == 0 ? (match != none ? TetrinetServerState::VACANT : TetrinetServerState::OCCUPIED) : TetrinetServerState::STOPPED);
}

const optional<const Match&> TetrinetServerPool::TetrinetServer::GetMatch() const
{
  return match;
}

void TetrinetServerPool::TetrinetServer::SetMatch(const Match& match)
{
  if (state != TetrinetServerState::VACANT)
    throw runtime_error("Server not vacant or stopped");
  
  this->match = match;
  state = TetrinetServerState::OCCUPIED;
  //onCompleteConnection = match.AddOnComplete(Match::CompleteSignal::slot_type(bind(&TetrinetServerPool::TetrinetServer::ClearMatch, this, _1)));
}

void TetrinetServerPool::TetrinetServer::ClearMatch(const Match& match)
{
  if (!this->match)
    throw runtime_error("No match in progress");
  if (&(this->match.value()) != &match)
    throw runtime_error("Wrong match event received");
  
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
  std::system(startCmd.c_str());
  if (!exists(pidFile))
    throw runtime_error("PID file does not exist");
  pidStream.open(pidFile);

  string pidS;
  getline(pidStream, pidS);
  pid = lexical_cast<pid_t>(pidS);
}

void TetrinetServerPool::TetrinetServer::Stop()
{
  std::system(stopCmd.c_str());
}
