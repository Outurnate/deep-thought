#ifndef MATCH_HPP
#define MATCH_HPP

#include <Wt/Dbo/Dbo>
#include <string>

#include "DeepThoughtForward.hpp"

class Match
{
public:
  Match() = default;

  template<typename Action>
  void persist(Action& a)
  {
    Wt::Dbo::belongsTo(a, owner, "generation");
    Wt::Dbo::field(a, genomeA, "a");
    Wt::Dbo::field(a, genomeB, "b");
    Wt::Dbo::field(a, isGenomeAWinner, "a_won");
    Wt::Dbo::field(a, complete, "complete");
    Wt::Dbo::field(a, scoreA, "a_score");
    Wt::Dbo::field(a, scoreB, "b_score");
  }

  // DBO Fields
  Wt::Dbo::ptr<Generation> owner;
  Wt::Dbo::ptr<Genome> genomeA;
  Wt::Dbo::ptr<Genome> genomeB;
  bool isGenomeAWinner;
  bool complete;
  int scoreA;
  int scoreB;
};

#endif
