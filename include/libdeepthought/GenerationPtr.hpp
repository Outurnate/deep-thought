#ifndef GENERATIONPTR_HPP
#define GENERATIONPTR_HPP

#include <Wt/Dbo/Dbo>
#include <stack>

#include "DeepThoughtForward.hpp"

#include "GenomePtr.hpp"

class GenerationPtr : private Wt::Dbo::ptr<Generation>
{
  friend class PopulationPtr;
public:
  GenerationPtr(PopulationPtr& population);
  GenerationPtr(const Wt::Dbo::ptr<Generation>& generation);
  
  size_t GenomeCount() const;
  GenerationPtr CreateNextGeneration() const;
  void CreateInitialGeneration();
  std::vector<MatchPtr> GetMatches();
private:
  void buildMatches();
};

#endif
