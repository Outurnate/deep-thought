#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Simple
#include <boost/test/unit_test.hpp>

int math(int a, int b)
{
  return a + b;
}

BOOST_AUTO_TEST_CASE(mathStillWorks)
{
  BOOST_CHECK(math(2, 2) == 4);
}
