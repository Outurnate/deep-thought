#include "AIManager.hpp"

#include <log4cxx/logger.h>

using namespace boost;
using namespace log4cxx;

AIManager::AIManager()
{
  addEngine(new AIEngine("DeepThought", -1.0f, -1.0f, -1.0f, 20.0f, this, Logger::getLogger("engines.DeepThought")));
  addEngine(new AIEngine("Hactar", -1.0f, -1.0f, -1.0f, 20.0f, this, Logger::getLogger("engines.Hactar")));
  addEngine(new AIEngine("HAL9000", -1.0f, -1.0f, -1.0f, 20.0f, this, Logger::getLogger("engines.HAL9000")));
  addEngine(new AIEngine("Data", -1.0f, -1.0f, -1.0f, 20.0f, this, Logger::getLogger("engines.Data")));
  addEngine(new AIEngine("WOPR", -1.0f, -1.0f, -1.0f, 20.0f, this, Logger::getLogger("engines.WOPR")));
}

AIManager::~AIManager()
{
}

void AIManager::Start()
{
  while (true); // you spin me right round
}

void AIManager::RegisterStatusHandler(IFieldStatusHandler* handler) // called by client
{
  handlers.push_back(handler);
}

void AIManager::SendEngineToChannel(AIEngine* engine, const std::string channel)
{
  engine->JoinChannel(channel);
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
