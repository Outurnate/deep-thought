#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ServerTest
#include <boost/test/unit_test.hpp>

#include <TetrinetServerPool.hpp>

constexpr uint32_t FromOctets(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
  return ((uint32_t)a << 24) + ((uint32_t)b << 16) + ((uint32_t)c << 8) + ((uint32_t)d);
}

struct ServerTestPool
{
  TetrinetServerPool p;

  ServerTestPool() : p("testpool", 5, FromOctets(127, 0, 0, 1), 31415, "%d/game.pid", "./start.sh %d", "./stop.sh %d")
  {
    BOOST_TEST_MESSAGE("setup pool");
  }

  ~ServerTestPool()
  {
    BOOST_TEST_MESSAGE("teardown pool");
  }
};

BOOST_FIXTURE_TEST_SUITE(ServerTest, ServerTestPool)

BOOST_AUTO_TEST_CASE(sanityCheck)
{
  BOOST_TEST_MESSAGE(p.GetStartedServers());
  BOOST_CHECK(p.GetStartedServers() == 0);
  BOOST_CHECK(p.GetActiveServers() == 0);
  BOOST_CHECK(p.IsServerAvailable());
}

BOOST_AUTO_TEST_CASE(startServers)
{
  p.StartServers();
  BOOST_CHECK(p.GetStartedServers() == p.GetServerCapacity());
}

BOOST_AUTO_TEST_SUITE_END()
