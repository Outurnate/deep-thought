#ifndef NULLSOLVER_HPP
#define NULLSOLVER_HPP

#include "DeepThoughtForward.hpp"

#include "SolverBase.hpp"

class NullSolver : public SolverBase
{
public:
  NullSolver(std::vector<MatchPtr> matches);
  virtual MatchResult RunMatch(const GenomePtr& a, const GenomePtr& b, unsigned matchId) override;
};

#endif
