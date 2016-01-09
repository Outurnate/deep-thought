#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <Wt/Dbo/Dbo>
#include <string>

class Generation;

/**
 * Represents a set of generations and their members
 */
class Population
{
public:
  /**
   * Retrieve a list of loaded generations
   */
  const Wt::Dbo::ptr<Wt::Dbo::collection<Wt::Dbo::ptr<Generation>>> GetGenerations() const;
  
  template<typename Action>
  void persist(Action& a)
  {
    Wt::Dbo::hasMany(a, generations, Wt::Dbo::ManyToOne, "population");
  }
private:
  Wt::Dbo::collection<Wt::Dbo::ptr<Generation>> generations;
};

#endif
