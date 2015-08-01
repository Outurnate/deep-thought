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

  // A - trigram
  // B - single char next
  // C - total
                 /*AAAAAAA*/           /*BBBBBBB*/  /*CCCCCCCCC*/
typedef std::map<std::string, std::map<std::string, unsigned long>> _corpus;
typedef std::map<std::string, std::map<std::string, double>>        _fcorpus;

class MarkovCorpus
{
public:
  MarkovCorpus();
  virtual ~MarkovCorpus();

  const _fcorpus* GetCorpus();
  
  static MarkovCorpus FromStream(std::istream& in);
private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive& ar, const unsigned version) { ar & BOOST_SERIALIZATION_NVP(corpus); }

  inline static void checkEntry(_corpus& c, std::string trigram, std::string entry);
  
  std::map<std::string, std::map<std::string, double>> corpus;
};

class MarkovGenerator
{
public:
  MarkovGenerator(MarkovCorpus* corpus);
  virtual ~MarkovGenerator();

  std::string GenerateName();
private:
  inline std::string weightedEntry(std::string trigram);
  
  MarkovCorpus* corpus;
  boost::mt11213b* mersenne;
  boost::variate_generator<boost::mt11213b, boost::uniform_int<> > *corpusDist, *nameLength;
};

#endif
