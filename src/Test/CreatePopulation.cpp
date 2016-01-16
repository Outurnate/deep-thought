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

  ptr<Population> population(manager.CreatePopulation("test"));
  auto populations = manager.GetPopulations();
  BOOST_REQUIRE_EQUAL(populations.size(), 1);
  {
    auto t(manager.InitiateTransaction());
    auto generations = (*populations.begin())->GetGenerations();
    BOOST_REQUIRE_EQUAL(generations.size(), 1);
  }
}

BOOST_AUTO_TEST_SUITE_END()
