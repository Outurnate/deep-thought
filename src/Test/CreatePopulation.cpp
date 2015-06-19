#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Simple
#include <boost/test/unit_test.hpp>

#include <Population.hpp>

BOOST_AUTO_TEST_CASE(createNewPopulation)
{
  Population* population = new Population("TestPopulation");
  unsigned populationSize = population->GetGenerations()->size();
  delete population;
  population = new Population("TestPopulation"); // should load data
  BOOST_REQUIRE_EQUAL(populationSize, population->GetGenerations()->size());
  delete population;
}
