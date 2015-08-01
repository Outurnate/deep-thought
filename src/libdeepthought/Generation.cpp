#include "libdeepthought/Generation.hpp"

#include <log4cxx/logger.h>
#include <soci/sqlite3/soci-sqlite3.h>

using namespace std;
using namespace log4cxx;
using namespace soci;

Generation::Generation(string connectionString)
  : genomes(new vector<Genome>(Generation::Size)),
    connectionString(connectionString),
    sql(new session(sqlite3, connectionString))
{
}

Generation::~Generation()
{
  delete genomes;
  delete sql;
}

const vector<Genome>* Generation::GetGenomes() const
{
  return genomes;
}

vector<Match> Generation::GetTrainingMatches() const
{
}

void Generation::commit() const
{
  for (Genome genome : *genomes)
    *sql << "INSERT OR REPLACE INTO genomes VALUES (:genome);", use(genome);
}

void soci::type_conversion<Generation>::from_base(values const& v, indicator ind, Generation& g)
{
  g.id = v.get<int>("id");
  for (unsigned i = 0; i < Generation::Size; ++i)
  {
    int genomeId = v.get<int>(((ostringstream&)(ostringstream() << "column" << i)).str());
    Genome genome;
    *g.sql << "SELECT * FROM genomes WHERE id = " << genomeId << ";", into(genome);
    g.genomes->push_back(genome);
  }
}

void soci::type_conversion<Generation>::to_base(const Generation& g, values& v, indicator& ind)
{
  v.set("id", g.id);
  for (unsigned i = 0; i < Generation::Size; ++i)
  {
    v.set<int>(((ostringstream&)(ostringstream() << "column" << i)).str(), (*g.genomes)[i].id);
    g.commit();
  }
}
