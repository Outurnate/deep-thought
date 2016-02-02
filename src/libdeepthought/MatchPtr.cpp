#include "libdeepthought/MatchPtr.hpp"

#include "libdeepthought/Match.hpp"
#include "libdeepthought/MatchResult.hpp"
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

void MatchPtr::SetResult(const MatchResult& result)
{
  this->modify()->winner = result.winnerIsGenomeA ? (*this)->genomeA : (*this)->genomeB;
  this->modify()->scoreA = result.scoreA;
  this->modify()->scoreB = result.scoreB;
  this->modify()->complete = true;
}

const GenomePtr MatchPtr::GetA() const
{
  return GenomePtr((*this)->genomeA);
}

const GenomePtr MatchPtr::GetB() const
{
  return GenomePtr((*this)->genomeB);
}
