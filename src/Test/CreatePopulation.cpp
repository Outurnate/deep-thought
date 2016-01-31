#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PopulationTest
#include <boost/test/unit_test.hpp>

#include "libdeepthought/DeepThought.hpp"

using namespace std;
using namespace Wt::Dbo;

BOOST_AUTO_TEST_SUITE(PopulationTest)

BOOST_AUTO_TEST_CASE(createNewPopulation)
{
  Configuration config;
  config.connectionString = "";
  AIManager manager(config);

  auto t(manager.InitiateTransaction());
  PopulationPtr population(manager, "test", 100, 25, 25, 50);
  BOOST_WARN_EQUAL(population.GetName(), std::string("test"));
  auto populations = manager.GetPopulations();
  BOOST_REQUIRE_EQUAL(populations.size(), 1);
  BOOST_REQUIRE_EQUAL(population.GenerationCount(), 1);
  BOOST_REQUIRE_EQUAL(population.GetCurrentGeneration()->GenomeCount(), 100);
}

BOOST_AUTO_TEST_CASE(smallPopTest)
{
  Configuration config;
  config.connectionString = "test";
  AIManager manager(config);
  
  auto t(manager.InitiateTransaction());
  PopulationPtr population(manager, "test", 100, 25, 25, 50);
  population.GetCurrentGeneration()->CreateInitialGeneration();
  auto matches = population.GetCurrentGeneration()->GetMatches();
  BOOST_REQUIRE(!matches.empty());
  NullSolver solver(matches);
  solver.Run();
}

BOOST_AUTO_TEST_SUITE_END()
