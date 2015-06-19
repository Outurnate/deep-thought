#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <string>
#include <list>
#include <boost-optional.h>
#include <soci/soci.h>

#include "Generation.hpp"

/**
 * Represents a set of generations and their members
 */
class Population
{
public:
  /**
   * Load the population from the specified connection by name
   * Connection strings will exist in config with defined names
   * Will create the database if it does not exist
   */
  explicit Population(const std::string name);
  virtual ~Population();

  /**
   * Retrieve a list of loaded generations
   */
  std::list<Generation> const* GetGenerations() const;
  /**
   * Retrieves training status for current population
   */
  const bool IsTrained() const;
private:
  void commit();
  void init();

  std::list<Generation>* generations;
  soci::session* sql;
};

#endif
