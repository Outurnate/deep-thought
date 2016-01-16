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
  return (*this)->generations.size();
}

const unique_ptr<const GenerationPtr> PopulationPtr::Top() const
{
  return make_unique<const GenerationPtr>((*this)->generations
                                          .find().orderBy("order descending").limit(1).resultValue());
}

const unique_ptr<GenerationPtr> PopulationPtr::Top()
{
  return make_unique<const GenerationPtr>((*this).modify()->generations
                                          .find().orderBy("order descending").limit(1).resultValue());
}
