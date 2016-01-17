#include "libdeepthought/PopulationPtr.hpp"

#include "libdeepthought/AIManager.hpp"
#include "libdeepthought/GenerationPtr.hpp"
#include "libdeepthought/Population.hpp"

using namespace std;
using namespace Wt::Dbo;

PopulationPtr::PopulationPtr(AIManager& manager, const string& name)
  : PopulationPtr(manager.CreatePopulation(name))
{
}

PopulationPtr::PopulationPtr(const ptr<Population>& population)
  : ptr<Population>(population)
{
}

const string& PopulationPtr::GetName() const
{
  return (*this)->name;
}

void PopulationPtr::SetName(const string& name)
{
  (*this).modify()->name = name;
}

size_t PopulationPtr::GenerationCount() const
{
  updateGenerations();
  return generationPtrs->size();
}

PopulationPtr::iterator PopulationPtr::begin()
{
  updateGenerations();
  return generationPtrs->begin();
}

PopulationPtr::iterator PopulationPtr::end()
{
  updateGenerations();
  return generationPtrs->end();
}

PopulationPtr::const_iterator PopulationPtr::cbegin() const
{
  updateGenerations();
  return generationPtrs->cbegin();
}

PopulationPtr::const_iterator PopulationPtr::cend() const
{
  updateGenerations();
  return generationPtrs->cend();
}

void PopulationPtr::updateGenerations() const
{
  if (!generationPtrs)
  {
    generationPtrs = vector<GenerationPtr>();
    auto gens = (*this)->generations.find().orderBy("order ascending").resultList();
    for (ptr<Generation> gen : gens)
      generationPtrs->emplace(generationPtrs->end(), gen);
  }
}
