#include "Population.hpp"

#include <soci/sqlite3/soci-sqlite3.h>

using namespace std;
using namespace soci;
using namespace boost;

Population::Population(const string name)
  : generations(new list<Generation>()),
    sql(new session(sqlite3, name))
{
  int tableExists;
  (*sql) << "SELECT count(*) FROM sqlite_master WHERE name ='generations' and type='table';", into(tableExists);

  if (!tableExists)
    init();
  
  int count;
  (*sql) << "SELECT count(*) FROM generations;", into(count);
  if (count > 0)
  {
    vector<int> ids(count);
    (*sql) << "SELECT id FROM generations", into(ids);
    for (int i = 0; i < count; ++i)
    {
      Generation gen;
      (*sql) << "SELECT * FROM generations WHERE id = " << ids[i] << ";", into(gen);
      generations->push_back(gen);
    }
  }
}

Population::~Population()
{
  delete generations;
}

void Population::init()
{
  (*sql) << "CREATE TABLE 'generations' (\
               'id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL\
             );";
  generations->push_back(Generation());
  commit();
}

list<Generation> const* Population::GetGenerations() const
{
  return generations;
}

void Population::commit()
{
  for (Generation gen : *generations)
    (*sql) << "INSERT OR REPLACE INTO generations VALUES (:generation);", use(gen);
}
