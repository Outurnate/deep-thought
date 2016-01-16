#ifndef GENERATIONPTR_HPP
#define GENERATIONPTR_HPP

#include <Wt/Dbo/Dbo>

#include "DeepThoughtForward.hpp"

class GenerationPtr : private boost::noncopyable, private Wt::Dbo::ptr<Generation>
{
public:
  GenerationPtr(AIManager& manager, PopulationPtr& population);
  GenerationPtr(const Wt::Dbo::ptr<Generation>& generation);
  
  size_t GenomeCount() const;
};

#endif
