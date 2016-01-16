#include "libdeepthought/AIManager.hpp"

#include <Wt/Dbo/Exception>
#include <Wt/Dbo/collection>
#include <Wt/Dbo/backend/Sqlite3>

#include "libdeepthought/Population.hpp"
#include "libdeepthought/Generation.hpp"
#include "libdeepthought/Genome.hpp"
#include "libdeepthought/Configuration.hpp"

using namespace std;
using namespace Wt::Dbo;
using namespace Wt::Dbo::backend;

AIManager::AIManager(const Configuration& config)
  : backend(make_unique<FixedSqlConnectionPool>(new Sqlite3(config.connectionString), 10)),
    session(make_unique<Session>()),
    config(config)
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

ptr<Population> AIManager::CreatePopulation(const string& name)
{
  auto population = new Population();
  population->name = name;
  
  return session->add(population);
}

ptr<Generation> AIManager::CreateGeneration(ptr<Population> population)
{
  Generation* generation = new Generation();
  generation->order = population->generations.size();

  auto generationPtr = session->add(generation);
  population.modify()->generations.insert(generationPtr);
  
  return generationPtr;
}

collection<ptr<Population>> AIManager::GetPopulations()
{
  return session->find<Population>();
}

const Configuration& AIManager::GetConfig() const
{
  return config;
}
