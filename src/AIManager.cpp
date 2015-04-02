#include "AIManager.hpp"

#include <log4cxx/logger.h>

using namespace boost;
using namespace log4cxx;

AIManager::AIManager() : population(0)
{
  supervisor = new Supervisor(this);
}

AIManager::~AIManager()
{
  delete supervisor;
  delete population;
}

void AIManager::RegisterStatusHandler(IFieldStatusHandler* handler) // called by client
{
  handlers.push_back(handler);
}

void AIManager::SendEngineToChannel(AIEngine* engine, const std::string channel)
{
  engine->JoinChannel(channel);
}

void AIManager::LoadPopulation(const std::string name)
{
  population = new Population(name);
  if (!population->IsTrained())
    population->Start(this);
  //start tick loop
}

void AIManager::QueueMatch(Match* match)
{
}

void AIManager::PopulationTick()
{
  //pseudo:
  //foreach engine
  //if done
  //find results, send to match
  //foreach channel in channels
  //if free
  //pop match queue
}

void AIManager::addEngine(AIEngine* engine)
{
  thread* engineThread = new thread(&AIEngine::Run, engine); // will dealloc with thread group
  engineThreads.add_thread(engineThread);
}

void AIManager::removeEngine(AIEngine* engine)
{
  engine->Stop();
  delete engine;
}

void AIManager::statusHandler(AIStatus status) // called from engine
{
  for (IFieldStatusHandler* handler : handlers)
    handler->HandleStatus(status);
}
