#ifndef GENERATION_HPP
#define GENERATION_HPP

#include <Wt/Dbo/Dbo>
#include <string>
#include <vector>

#include "DeepThoughtForward.hpp"

#include "Population.hpp"
#include "Genome.hpp"

class Generation
{
public:
  Generation() = default;
  
  template<typename Action>
  void persist(Action& a)
  {
    Wt::Dbo::field(a, order, "order");
    Wt::Dbo::belongsTo(a, owner, "population");
  }

  // DBO fields
  Wt::Dbo::ptr<Population> owner;
  Wt::Dbo::collection<Wt::Dbo::ptr<Genome> > genomes;
  int order; // TODO
};

#endif
