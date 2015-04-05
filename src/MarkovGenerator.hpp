#ifndef MARKOVGENERATOR_HPP
#define MARKOVGENERATOR_HPP

#include <iostream> // TODO tmp
#include <map>
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/map.hpp>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>

template <typename K, typename V>
struct map_v
{
  typedef const V result_type;
  const V& operator()(const std::pair<K, V>& pair) const { return pair.second; }
};

class MarkovCorpus
{
public:
  MarkovCorpus();
  virtual ~MarkovCorpus();

  const std::map<std::string, std::map<std::string, double>>* GetCorpus();
  
  static MarkovCorpus FromStream(std::istream& in);
private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive& ar, const unsigned version) { ar & BOOST_SERIALIZATION_NVP(corpus); }
  
  std::map<std::string, std::map<std::string, double>> corpus;
};

class MarkovGenerator
{
public:
  MarkovGenerator(MarkovCorpus* corpus);
  virtual ~MarkovGenerator();

  std::string GenerateName();
private:
  MarkovCorpus* corpus;
  boost::mt11213b* mersenne;
  boost::variate_generator<boost::mt11213b, boost::uniform_int<> > *corpusDist, *nameLength;
};

#endif
