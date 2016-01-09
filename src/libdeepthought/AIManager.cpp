#include "libdeepthought/AIManager.hpp"

#include <Wt/Dbo/backend/Sqlite3>

#include "libdeepthought/Population.hpp"
#include "libdeepthought/Generation.hpp"
#include "libdeepthought/Genome.hpp"

using namespace std;
using namespace Wt::Dbo;
using namespace Wt::Dbo::backend;

AIManager::AIManager()
  : backend(make_unique<FixedSqlConnectionPool>(new Sqlite3("test.db"), 1))
{
}

std::shared_ptr<Session> AIManager::PrepareSession()
{
  std::shared_ptr<Session> session(make_shared<Session>());
  session->setConnectionPool(*backend);
  session->mapClass<Population>("population");
  session->mapClass<Generation>("generation");
  session->mapClass<Genome>("genome");
  session->createTables();
  return session;
}
