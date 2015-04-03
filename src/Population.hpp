#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <string>
#include <list>

class Population;
class Generation; // TODO buhbye

#include "Generation.hpp"
#include "AIManager.hpp"

/**
 * Represents a set of generations and their members
 */
class Population
{
  friend class AIManager;

public:
  /**
   * Load the population from the specified connection by name
   * Connection strings will exist in config with defined names
   * Will create the database if it does not exist
   */
  Population(std::string name, AIManager* manager);
  virtual ~Population();

  /**
   * Retrieve a list of loaded generations
   */
  const std::list<Generation>* GetGenerations() const;
  /**
   * Retrieves training status for current population
   */
  bool IsTrained();
private:
  std::list<Generation>* generations;

  void commit();
  void stateHandler(AIEngine* engine, AIState state);

  AIManager* manager;
};

#endif
