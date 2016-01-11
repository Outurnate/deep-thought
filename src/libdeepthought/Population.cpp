#include "libdeepthought/Population.hpp"

#include "libdeepthought/AIManager.hpp"

using namespace std;
using namespace Wt::Dbo;

ptr<Population> Population::CreatePopulation(AIManager& manager, const std::string& name)
{
  unique_ptr<Transaction> transaction(manager.InitiateTransaction());
  
  Population* population = new Population();
  population->name = name;

  return manager.RegisterPopulation(population);
}

const collection<ptr<Generation>> Population::GetGenerations() const
{
  return generations;
}

void Population::InitializeGenerations(AIManager& manager)
{
  //Generation::CreateGeneration(manager);
}
