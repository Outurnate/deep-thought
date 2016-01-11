#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <Wt/Dbo/Dbo>
#include <string>

#include "DeepThoughtForward.hpp"

#include "Generation.hpp"

/**
 * Represents a set of generations and their members
 */
class Population
{
public:
  static Wt::Dbo::ptr<Population> CreatePopulation(AIManager& manager, const std::string& name);
  const Wt::Dbo::collection<Wt::Dbo::ptr<Generation>> GetGenerations() const;
  void InitializeGenerations(AIManager& manager);
  
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
