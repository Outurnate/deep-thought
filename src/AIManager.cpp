#include "AIManager.hpp"

using namespace boost;

AIManager::AIManager(int poolSize)
{
  for (int i = 0; i < poolSize; ++i)
  {
    addEngine(new AIEngine("Test", -1.0f, -1.0f, -1.0f, 20.0f, this));
  }
}

AIManager::~AIManager()
{
  
}

void AIManager::Start()
{
  
}

void AIManager::RegisterStatusHandler(void (*handler)(AIStatus)) // called by client
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
  for (void (*handler)(AIStatus)& : handlers)
  {
    handler(status);
  }
}
