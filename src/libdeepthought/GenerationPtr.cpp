#include "libdeepthought/GenerationPtr.hpp"

#include "libdeepthought/AIManager.hpp"
#include "libdeepthought/PopulationPtr.hpp"
#include "libdeepthought/Generation.hpp"

using namespace Wt::Dbo;

GenerationPtr::GenerationPtr(AIManager& manager, PopulationPtr& population)
  : GenerationPtr(manager.CreateGeneration(population))
{
}

GenerationPtr::GenerationPtr(const ptr<Generation>& generation)
  : ptr<Generation>(generation)
{
}
  
size_t GenerationPtr::GenomeCount() const
{
  return (*this)->genomes.size();
}
