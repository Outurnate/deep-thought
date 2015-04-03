#ifndef AIMANAGER_HPP
#define AIMANAGER_HPP

#include <vector>
#include <boost/thread.hpp>

class AIManager;
class Supervisor;
class Match; // TODO go away

#include "AIEngine.hpp"
#include "AIStatus.hpp"
#include "Supervisor.hpp"
#include "Population.hpp"
#include "Match.hpp"

class IFieldStatusHandler
{
public:
  virtual ~IFieldStatusHandler() {}
  virtual void HandleStatus(AIStatus status) = 0;
};

class AIManager
{
  friend class AIEngine;

public:
  AIManager();
  virtual ~AIManager();
  
  void RegisterStatusHandler(IFieldStatusHandler* handler);
  void SendEngineToChannel(AIEngine* engine, const std::string channel);
  void LoadPopulation(const std::string name);
  void QueueMatch(Match* match);
  void PopulationTick();
private:
  std::vector<AIEngine> engines;
  std::vector<IFieldStatusHandler*> handlers;
  boost::thread_group engineThreads;

  void addEngine(AIEngine* engine);
  void removeEngine(AIEngine* engine);

  void statusHandler(AIStatus status);
  void stateHandler(AIEngine* engine, AIState state);

  Supervisor* supervisor;
  Population* population;
};

#endif
