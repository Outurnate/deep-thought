#ifndef NULLSOLVER_HPP
#define NULLSOLVER_HPP

#include "DeepThoughtForward.hpp"

#include "SolverBase.hpp"

class NullSolver : public SolverBase
{
public:
  NullSolver(std::vector<MatchPtr> matches);
  virtual void RunMatch(MatchPtr& match);
};

#endif
