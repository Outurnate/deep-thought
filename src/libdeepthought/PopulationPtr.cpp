#include "libdeepthought/PopulationPtr.hpp"

#include "libdeepthought/AIManager.hpp"
#include "libdeepthought/GenerationPtr.hpp"
#include "libdeepthought/Population.hpp"

using namespace std;
using namespace Wt::Dbo;

PopulationPtr::PopulationPtr(AIManager& manager, const string& name, int generationSize, int elitists, int crossovers, int mutants)
  : PopulationPtr(manager.CreatePopulation(name))
{
  this->modify()->E = elitists;
  this->modify()->C = crossovers;
  this->modify()->M = mutants;
  this->modify()->GenerationSize = generationSize;
  GenerationPtr generation(*this); // create initial generation
  generation.CreateInitialGeneration();
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
  return (*this)->generations.size();
}

const unique_ptr<GenerationPtr> PopulationPtr::GetCurrentGeneration() const
{
  return make_unique<GenerationPtr>((*this)->generations.find().orderBy("\"order descending\"").limit(1).resultValue());
}
