#include "libdeepthought/NullSolver.hpp"

#include "libdeepthought/MatchPtr.hpp"

using namespace std;

NullSolver::NullSolver(vector<MatchPtr> matches)
  : SolverBase(matches)
{
}

MatchResult NullSolver::RunMatch(const Genome& a, const Genome& b, unsigned matchId)
{
  MatchResult result;
  result.winnerIsGenomeA = true;
  result.scoreA = 0;
  result.scoreB = 10;
  result.matchId = matchId;
  return result;
}
