#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Simple
#include <boost/test/unit_test.hpp>

#include <Population.hpp>

BOOST_AUTO_TEST_CASE(createNewPopulation)
{
  Population* population = new Population("TestPopulation", NULL);
  population->Init();
  unsigned populationSize = population->GetGenerations()->size();
  delete population;
  Population* population2 = new Population("TestPopulation", NULL); // should load data
  BOOST_REQUIRE_EQUAL(populationSize, population2->GetGenerations()->size());
  delete population2;
}
