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
  static Wt::Dbo::ptr<Generation> CreateGeneration(AIManager& manager);
  
  const Wt::Dbo::ptr<Wt::Dbo::collection<Wt::Dbo::ptr<Genome>>> GetGenomes() const;
  std::vector<Match> GetTrainingMatches() const;

  template<typename Action>
  void persist(Action& a)
  {
    Wt::Dbo::belongsTo(a, owner, "population");
  }
private:
  Wt::Dbo::ptr<Population> owner;
  Wt::Dbo::collection<Wt::Dbo::ptr<Genome>> genomes;
};

#endif
