#ifndef MATCHPTR_HPP
#define MATCHPTR_HPP

#include <Wt/Dbo/Dbo>

#include "DeepThoughtForward.hpp"

class MatchPtr : private Wt::Dbo::ptr<Match>
{
  friend class SolverBase;
public:
  MatchPtr(AIManager& manager, GenerationPtr& generation, GenomePtr& genomeA, GenomePtr& genomeB);
  MatchPtr(const Wt::Dbo::ptr<Match>& match);

  void SetResult(const MatchResult& result);
};

#endif
