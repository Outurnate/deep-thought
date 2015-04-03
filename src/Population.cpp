#include "Population.hpp"

using namespace std;

Population::Population(std::string name, AIManager* manager) : manager(manager)
{
  generations = new list<Generation>();
  //dummy generations
  generations->insert(generations->end(), Generation(100));
}

Population::~Population()
{
  delete generations;
}

const list<Generation>* Population::GetGenerations() const
{
}

void Population::stateHandler(AIEngine* engine, AIState state)
{
}
