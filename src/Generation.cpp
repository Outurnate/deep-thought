#include "Generation.hpp"

#include <log4cxx/logger.h>

using namespace std;
using namespace log4cxx;

Generation::Generation()
{
}

Generation::~Generation()
{
  //delete genomes;
}

const map<string, Genome>* Generation::GetGenomes() const
{
  return genomes;
}

vector<Match> Generation::GetTrainingMatches() const
{
}
