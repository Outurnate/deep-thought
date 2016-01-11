#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PopulationTest
#include <boost/test/unit_test.hpp>

#include "libdeepthought/DeepThought.hpp"

using namespace std;
using namespace Wt::Dbo;

BOOST_AUTO_TEST_SUITE(PopulationTest)

BOOST_AUTO_TEST_CASE(createNewPopulation)
{
/*  Configuration config;
  config.connectionString = "";
  AIManager manager(config);
  unique_ptr<Transaction> transaction(manager.InitiateTransaction());
  {
    ptr<Population> population(Population::CreatePopulation(manager, "test"));
    auto populations = manager.GetPopulations();
    BOOST_REQUIRE_EQUAL(populations.size(), 1);
    }*/
}

BOOST_AUTO_TEST_CASE(initializePopulation)
{
  /*Configuration config;
  config.connectionString = "";
  AIManager manager(config);
  unique_ptr<Transaction> transaction(manager.InitiateTransaction());
  {
    ptr<Population> population(Population::CreatePopulation(manager, "test"));
    population.modify()->InitializeGenerations(manager);
    }*/
}

BOOST_AUTO_TEST_SUITE_END()
