#include "libdeepthought/Supervisor.hpp"

#include "Constants.hpp"

#include <log4cxx/logger.h>

using namespace log4cxx;

Supervisor::Supervisor(AIManager* manager) : TetrinetClient("OverseerOwen", Logger::getLogger("engineManager")), manager(manager)
{
  this->Run();
}

Supervisor::~Supervisor()
{
  this->Stop();
}

void Supervisor::ProcessCommand(TetrinetMessage message, std::deque<std::string>* tokens)
{
  switch (message)
  {
  case TetrinetMessage::PLINE:
    try
    {
      switch (djb2(tokens->at(1).c_str()))
      {
      case djb2("?begin"):
	try
	{
	  manager->LoadPopulation(tokens->at(2));
	}
	catch (std::out_of_range)
	{
	  // Malformed command
	}
	break;
      }
    }
    catch (std::out_of_range)
    {
      // gulp
    }
    break;
  }
}
