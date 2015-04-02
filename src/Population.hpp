#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <string>
#include <vector>

#include "Generation.hpp"

/**
 * Represents a set of generations and their members
 */
class Population
{
public:
  /**
   * Load the population from the specified connection
   * Will create the database if it does not exist
   */
  Population(std::string connection);
  virtual ~Population();

  /**
   * Retrieve a list of loaded generations
   */
  const std::vector<Generation>* GetGenerations() const;
private:
  std::vector<Generation*> generations;

  void commit();
};

#endif
