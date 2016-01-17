#ifndef GENERATIONPTR_HPP
#define GENERATIONPTR_HPP

#include <Wt/Dbo/Dbo>

#include "DeepThoughtForward.hpp"

#include "GenomePtr.hpp"

class GenerationPtr : private Wt::Dbo::ptr<Generation>
{
  typedef std::vector<GenomePtr>::iterator iterator;
  typedef std::vector<GenomePtr>::const_iterator const_iterator;
public:
  GenerationPtr(AIManager& manager, PopulationPtr& population);
  GenerationPtr(const Wt::Dbo::ptr<Generation>& generation);
  
  size_t GenomeCount() const;

  iterator begin();
  iterator end();

  const_iterator cbegin() const;
  const_iterator cend() const;
private:
  void updateGenomes();
  
  std::vector<GenomePtr> genomePtrs;
};

#endif
