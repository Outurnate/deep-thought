#include "libdeepthought/Population.hpp"

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
