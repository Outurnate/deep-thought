#include "Generation.hpp"

#include <log4cxx/logger.h>

using namespace std;
using namespace log4cxx;

Generation::Generation(int genomes)
{
  this->genomes = new map<string, Genome>();
  this->engines = new vector<AIEngine*>(genomes);
  for (unsigned i = 0; i < genomes; ++i)
  {
    Genome genome = Genome();
    this->genomes->insert(pair<string, Genome>(genome.Name, genome));
    this->engines->push_back(new AIEngine(genome, Logger::getLogger("engines." + genome.Name)));
  }
}

Generation::~Generation()
{
  delete genomes;
}

const map<string, Genome>* Generation::GetGenomes() const
{
  return genomes;
}

vector<Match> Generation::GetTrainingMatches()
{
  // Round-robin matches
  vector<Match> matches;
  for (AIEngine* engineA : *engines)
    for (AIEngine* engineB : *engines)
      if (engineA != engineB)
	matches.push_back(Match(engineA, engineB));
  return matches;
}
