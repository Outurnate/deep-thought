#include "Supervisor.hpp"

#include <log4cxx/logger.h>

using namespace log4cxx;

Supervisor::Supervisor() : TetrinetClient("Overseer Owen", Logger::getLogger("engineManager"))
{
  this->Run();
}

Supervisor::~Supervisor()
{
  this->Stop();
}

void Supervisor::ProcessCommand(TetrinetMessage message, std::deque<std::string>* tokens)
{
}

