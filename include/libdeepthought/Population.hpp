#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <Wt/Dbo/Dbo>
#include <string>

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

  /**
   * Retrieve a list of loaded generations
   */
  const std::shared_ptr<std::list<Generation>> GetGenerations() const;
  
  Wt::Dbo::collection<Wt::Dbo::ptr<Generation>> generations;
  
  template<typename Action>
  void persist(Action& a)
  {
    Wt::Dbo::hasMany(a, generations, Wt::Dbo::ManyToOne, "population");
  }
private:
};

#endif
