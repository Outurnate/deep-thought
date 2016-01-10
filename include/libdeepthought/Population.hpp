#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <Wt/Dbo/Dbo>
#include <string>

class Population;

#include "AIManager.hpp"
#include "Configuration.hpp"

class Generation;

/**
 * Represents a set of generations and their members
 */
class Population
{
public:
  static Wt::Dbo::ptr<Population> CreatePopulation(AIManager& manager, const std::string& name);
  /**
   * Retrieve a list of loaded generations
   */
  const Wt::Dbo::collection<Wt::Dbo::ptr<Generation>> GetGenerations() const;
  
  template<typename Action>
  void persist(Action& a)
  {
    Wt::Dbo::hasMany(a, generations, Wt::Dbo::ManyToOne, "population");
    Wt::Dbo::field(a, name, "name");
  }
private:
  Wt::Dbo::collection<Wt::Dbo::ptr<Generation>> generations;

  std::string name;
};

#endif
