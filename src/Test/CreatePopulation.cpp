#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PopulationTest
#include <boost/test/unit_test.hpp>

#include <Population.hpp>

BOOST_AUTO_TEST_SUITE(PopulationTest)

BOOST_AUTO_TEST_CASE(createNewPopulation)
{
  Population* population = new Population("TestPopulationDeep");
  unsigned populationSize = population->GetGenerations()->size();
  unsigned genZeroGenomeCount = population->GetGenerations()->front().GetGenomes()->size();
  delete population;
  Population* populationTwo = new Population("TestPopulationDeep");
  BOOST_REQUIRE_EQUAL(populationSize, populationTwo->GetGenerations()->size());
  BOOST_REQUIRE_EQUAL(genZeroGenomeCount, populationTwo->GetGenerations()->front().GetGenomes()->size());
  delete populationTwo;
}

BOOST_AUTO_TEST_SUITE_END()
