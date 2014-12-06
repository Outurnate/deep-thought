#include "AIManager.hpp"

#include <log4cxx/logger.h>

using namespace boost;
using namespace log4cxx;

AIManager::AIManager()
{
  addEngine(new AIEngine("Test", -1.0f, -1.0f, -1.0f, 20.0f, this, Logger::getLogger("engines.Test")));
}

AIManager::~AIManager()
{

}

void AIManager::Start()
{

}

void AIManager::RegisterStatusHandler(IFieldStatusHandler* handler) // called by client
{
  handlers.push_back(handler);
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
