#include "Population.hpp"

#include <soci/sqlite3/soci-sqlite3.h>

#define TRANSACTION_SIZE 100

using namespace std;
using namespace soci;
using namespace boost;

Population::Population(std::string name, AIManager* manager) : manager(manager)
{
  generations = new list<Generation>();
  sql = new session(sqlite3, name);
  int genCount;
  (*sql) << "SELECT count(*) FROM sqlite_master WHERE name ='generations' and type='table';", into(genCount);
  isInit = genCount != 0;

  if (isInit)
  {
    // grab data in blocks of TRANSACTION_SIZE, then add them to local store
    vector<int> generationIds(TRANSACTION_SIZE);
    do
    {
      generationIds.clear();
      generationIds.resize(TRANSACTION_SIZE);
      (*sql) << "select id from generations", into(generationIds);
      for (int genId : generationIds)
        generations->push_back(Generation(genId));
    }
    while(generationIds.size() == TRANSACTION_SIZE);
  }
}

Population::~Population()
{
  delete generations;
}

void Population::Init()
{
  (*sql) << "CREATE TABLE 'generations' (\
               'id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL\
             );";
  generations->push_back(Generation());
  commit();
  isInit = true;
}

const list<Generation>* Population::GetGenerations() const
{
  return generations;
}

void Population::commit()
{
  foreach(Generation gen : generations)
    gen.commit();
}
