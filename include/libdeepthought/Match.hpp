#ifndef MATCH_HPP
#define MATCH_HPP

#include <Wt/Dbo/Dbo>
#include <string>

#include "DeepThoughtForward.hpp"

class Match
{
public:
  Match() = default;
  Match(const Wt::Dbo::ptr<Genome>& a, const Wt::Dbo::ptr<Genome>& b)
    : genomeA(a), genomeB(b) {}

  template<typename Action>
  void persist(Action& a)
  {
    Wt::Dbo::belongsTo(a, owner, "generation");
    Wt::Dbo::belongsTo(a, winner, "genome");
    Wt::Dbo::field(a, genomeA, "genomeA");
    Wt::Dbo::field(a, genomeB, "genomeB");
    Wt::Dbo::field(a, complete, "complete");
    Wt::Dbo::field(a, scoreA, "a_score");
    Wt::Dbo::field(a, scoreB, "b_score");
  }

  // DBO Fields
  Wt::Dbo::ptr<Generation> owner;
  Wt::Dbo::ptr<Genome> winner;
  Wt::Dbo::ptr<Genome> genomeA;
  Wt::Dbo::ptr<Genome> genomeB;
  bool complete;
  int scoreA;
  int scoreB;
};

#endif
