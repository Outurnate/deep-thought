#include "libdeepthought/Generation.hpp"

#include <log4cxx/logger.h>

using namespace std;
using namespace log4cxx;

Generation::Generation()
  : genomes(make_shared<vector<Genome>>(100))
{
}

const shared_ptr<vector<Genome>> Generation::GetGenomes() const
{
  return genomes;
}

vector<Match> Generation::GetTrainingMatches() const
{
}
