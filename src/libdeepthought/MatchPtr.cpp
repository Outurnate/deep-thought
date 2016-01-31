#include "libdeepthought/MatchPtr.hpp"

#include "libdeepthought/Match.hpp"
#include "libdeepthought/GenomePtr.hpp"

using namespace Wt::Dbo;

MatchPtr::MatchPtr(AIManager& manager, GenerationPtr& generation, GenomePtr& genomeA, GenomePtr& genomeB)
{
  this->modify()->genomeA = genomeA;
  this->modify()->genomeB = genomeB;
  this->modify()->complete = false;
}

MatchPtr::MatchPtr(const ptr<Match>& match)
  : ptr<Match>(match)
{
}

void MatchPtr::SetResult(bool winnerIsGenomeA, int scoreA, int scoreB)
{
  this->modify()->winner = winnerIsGenomeA ? (*this)->genomeA : (*this)->genomeB;
  this->modify()->scoreA = scoreA;
  this->modify()->scoreB = scoreB;
  this->modify()->complete = true;
}
