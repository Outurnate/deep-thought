#include "libdeepthought/Population.hpp"

#include <soci/sqlite3/soci-sqlite3.h>

using namespace std;
using namespace soci;
using namespace boost;

Population::Population(const string name)
  : generations(new list<Generation>()),
    connectionString(name),
    sql(new session(sqlite3, name))
{
  int tableExists;
  *sql << "SELECT count(*) FROM sqlite_master WHERE name ='generations' and type='table';", into(tableExists);

  if (!tableExists)
    init();
  
  int count;
  *sql << "SELECT count(*) FROM generations;", into(count);
  if (count > 0)
  {
    vector<int> ids(count);
    *sql << "SELECT id FROM generations", into(ids);
    for (int i = 0; i < count; ++i)
    {
      Generation gen(connectionString);
      *sql << "SELECT * FROM generations WHERE id = " << ids[i] << ";", into(gen);
      generations->push_back(gen);
    }
  }
}

Population::~Population()
{
  delete generations;
  delete sql;
}

void Population::init()
{
  stringstream columns;
  for (unsigned i = 0; i < Generation::Size; ++i)
    columns << ", 'genome" << i << "' INTEGER NOT NULL\n";
  *sql << "CREATE TABLE 'generations' ("
       <<    "'id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL"
       <<    columns.str()
       <<  ");"
       <<  "CREATE TABLE 'genomes' ("
       <<    "'id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
       <<    "B REAL NOT NULL,"
       <<    "G REAL NOT NULL,"
       <<    "R REAL NOT NULL,"
       <<    "C REAL NOT NULL"
       <<  ");";
  generations->push_back(Generation(connectionString));
  commit();
}

list<Generation> const* Population::GetGenerations() const
{
  return generations;
}

void Population::commit()
{
  for (Generation gen : *generations)
  {
    *sql << "INSERT OR REPLACE INTO generations VALUES (:generation);", use(gen);
    gen.commit();
  }
}
