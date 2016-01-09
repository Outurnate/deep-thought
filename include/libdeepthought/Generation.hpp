#ifndef GENERATION_HPP
#define GENERATION_HPP

#include <Wt/Dbo/Dbo>
#include <string>
#include <vector>

#include "Population.hpp"
#include "Genome.hpp"
#include "Match.hpp"

class Generation
{
public:
  /**
   * Retrieve a list of loaded genomes in this generation
   */
  const Wt::Dbo::ptr<Wt::Dbo::collection<Wt::Dbo::ptr<Genome>>> GetGenomes() const;
  /**
   * Creates matches for training
   */
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
