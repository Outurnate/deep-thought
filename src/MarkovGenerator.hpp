#ifndef MARKOVGENERATOR_HPP
#define MARKOVGENERATOR_HPP

#include <iostream>
#include <map>
#include <string>
#include <boost/serialization/access.hpp>

class MarkovCorpus
{
public:
  MarkovCorpus();
  virtual ~MarkovCorpus();

  const std::map<std::string, std::map<std::string, double>>* GetCorpus();
  
  static MarkovCorpus FromStream(const std::istream& in);
private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive& ar, const unsigned version) { ar & corpus; }
  
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
};

#endif
