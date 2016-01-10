#include "libdeepthought/AIManager.hpp"

#include <Wt/Dbo/backend/Sqlite3>

#include "libdeepthought/Population.hpp"
#include "libdeepthought/Generation.hpp"
#include "libdeepthought/Genome.hpp"

using namespace std;
using namespace Wt::Dbo;
using namespace Wt::Dbo::backend;

AIManager::AIManager(const Configuration& config)
  : backend(make_unique<FixedSqlConnectionPool>(new Sqlite3(config.connectionString), 10)),
    session(make_unique<Session>())
{
  session->setConnectionPool(*backend);
  session->mapClass<Population>("population");
  session->mapClass<Generation>("generation");
  session->mapClass<Genome>("genome");
  try
  {
    session->createTables();
  }
  catch (Exception& e)
  {
    // table exists
  }
}

unique_ptr<Transaction> AIManager::InitiateTransaction()
{
  return make_unique<Transaction>(*session);
}

ptr<Population> AIManager::RegisterPopulation(Population* population)
{
  return session->add(population);
}

collection<ptr<Population>> AIManager::GetPopulations()
{
  Transaction transaction(*session);
  return session->find<Population>();
}
