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
  population = new Population(name, this);
}

void AIManager::QueueMatch(Match* match)
{
}

// Called via population when status changes
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
  engine->BindManager(this);
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

void AIManager::stateHandler(AIEngine* engine, AIState state)
{
  if (population)
    population->stateHandler(engine, state);
}
