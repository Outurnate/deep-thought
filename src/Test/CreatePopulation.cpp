#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PopulationTest
#include <boost/test/unit_test.hpp>

#include "libdeepthought/Population.hpp"
#include "libdeepthought/AIManager.hpp"
#include "libdeepthought/Configuration.hpp"

using namespace std;
using namespace Wt::Dbo;

BOOST_AUTO_TEST_SUITE(PopulationTest)

BOOST_AUTO_TEST_CASE(createNewPopulation)
{
  Configuration config;
  config.connectionString = "test.db";
  AIManager manager(config);
  unique_ptr<Transaction> transaction(manager.InitiateTransaction());
  {
    ptr<Population> population(Population::CreatePopulation(manager, "test"));
    auto populations = manager.GetPopulations();
    BOOST_REQUIRE_EQUAL(populations.size(), 1);
  }
}

BOOST_AUTO_TEST_SUITE_END()
