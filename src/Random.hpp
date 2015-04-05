#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <limits>

class Random
{
private:
  Random()
  {
    mersenne = new boost::mt11213b(time(0));
    _zeroOne = new boost::uniform_real<>(0.0, 1.0);
    _zeroMax = new boost::uniform_int<>(0, std::numeric_limits<unsigned>::max());
    zeroOne = new boost::variate_generator<boost::mt11213b, boost::uniform_real<> >(*mersenne, *_zeroOne);
    zeroMax = new boost::variate_generator<boost::mt11213b, boost::uniform_int<> >(*mersenne, *_zeroMax);
  }
  virtual ~Random() { }
  Random(Random const&)         = delete;
  void operator=(Random const&) = delete;

  boost::mt11213b* mersenne;
  
  boost::uniform_real<>* _zeroOne;
  boost::uniform_int<>*  _zeroMax;
  boost::variate_generator<boost::mt11213b, boost::uniform_real<> >* zeroOne;
  boost::variate_generator<boost::mt11213b, boost::uniform_int<> >* zeroMax;
public:
  static Random& Get() { static Random instance; return instance; }

  inline double ZeroOne() { (*zeroOne)(); }
  inline int ZeroMax() { (*zeroMax)(); }
};

#endif
