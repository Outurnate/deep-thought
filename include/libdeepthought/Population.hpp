#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <Wt/Dbo/Dbo>
#include <string>

#include "DeepThoughtForward.hpp"

#include "Generation.hpp"
#include "Match.hpp"

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
    Wt::Dbo::field(a, GenerationSize, "size");
    Wt::Dbo::field(a, E, "e");
    Wt::Dbo::field(a, C, "c");
    Wt::Dbo::field(a, M, "m");
  }
  
  // DBO Fields
  std::string name;
  Wt::Dbo::collection<Wt::Dbo::ptr<Generation> > generations;
  int GenerationSize;
  int E;
  int C;
  int M;
};

#endif
