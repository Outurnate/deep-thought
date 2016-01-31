#include "libdeepthought/NullSolver.hpp"

#include "libdeepthought/MatchPtr.hpp"

using namespace std;

NullSolver::NullSolver(vector<MatchPtr> matches)
  : SolverBase(matches)
{
}

void NullSolver::RunMatch(MatchPtr& match)
{
  match.SetResult(true, 1000, 0);
}
