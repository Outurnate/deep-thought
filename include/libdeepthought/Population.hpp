#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <Wt/Dbo/Dbo>
#include <string>

#include "DeepThoughtForward.hpp"

#include "Generation.hpp"

/**
 * Core ORM class for a population
 */
class Population
{
public:
  Population() = default;
  
  template<typename Action>
  void persist(Action& a)
  {
    Wt::Dbo::hasMany(a, generations, Wt::Dbo::ManyToOne, "population");
    Wt::Dbo::field(a, name, "name");
  }
  
  // DBO Fields
  Wt::Dbo::collection<Wt::Dbo::ptr<Generation> > generations;
  std::string name;
};

#endif
