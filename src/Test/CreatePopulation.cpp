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
  config.connectionString = "test";
  AIManager manager(config);

  auto t(manager.InitiateTransaction());
  PopulationPtr population(manager, "test");
  BOOST_WARN_EQUAL(population.GetName(), std::string("test"));
  auto populations = manager.GetPopulations();
  BOOST_REQUIRE_EQUAL(populations.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
