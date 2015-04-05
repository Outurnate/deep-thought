#include "MarkovGenerator.hpp"

#include <iterator>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/random/discrete_distribution.hpp>

using namespace std;
using namespace boost;

MarkovGenerator::MarkovGenerator(MarkovCorpus* corpus) : corpus(corpus)
{
  mersenne = new boost::mt11213b(time(0));
  corpusDist = new boost::variate_generator<boost::mt11213b, boost::uniform_int<> >(*mersenne, boost::uniform_int<>(0, corpus->GetCorpus()->size()));
  nameLength = new boost::variate_generator<boost::mt11213b, boost::uniform_int<> >(*mersenne, boost::uniform_int<>(0, 7));
}

MarkovGenerator::~MarkovGenerator()
{
}

string MarkovGenerator::GenerateName()
{
  string name = next(corpus->GetCorpus()->begin(), (*corpusDist)())->first; // get a random trigram from corpus
  int length = 5 + (*nameLength)(); // 5-12 length
  while (name.length() != length)
  {
    map<string, double> entry = corpus->GetCorpus()->at(name.substr(name.length() - 3, 3));
    auto begin = make_transform_iterator(entry.begin(), map_v<string, double>()),
      end = make_transform_iterator(entry.end(), map_v<string, double>());
    random::discrete_distribution<> dist(begin, end);
    name += next(entry.begin(), dist(*mersenne))->first;
  }
  return name;
}

MarkovCorpus::MarkovCorpus()
{
}

MarkovCorpus::~MarkovCorpus()
{
}

const std::map<std::string, std::map<std::string, double>>* MarkovCorpus::GetCorpus()
{
  return &corpus;
}

MarkovCorpus MarkovCorpus::FromStream(istream& in)
{
  // A - trigram
  // B - single char next
  // C - total
      /*AA*/      /*BB*/  /*CCCCCCCCC*/
  map<string, map<string, unsigned long>> corpus;

  // Each line will contain one word
  for (string line; getline(in, line);)
    for (unsigned i = 0; i < (line.length() - 4); ++i)
    {
      string trigram = line.substr(i, 3);
      string next = line.substr(i + 3, 1);

      if (next.length() == 0)
	break;

      // Ensure keys exist
      if (!corpus.count(trigram))
	corpus.emplace(trigram, map<string, unsigned long>());
      if (!corpus.at(trigram).count(next))
	corpus.at(trigram).emplace(next, 0);

      // Increment
      ++corpus.at(trigram).at(next);
    }

  // Convert to frequency corpus
  map<string, map<string, double>> frequencyCorpus;
  for (pair<string, map<string, unsigned long>> trigramEntry : corpus)
  {
    unsigned long total = 0; // total keys
    for (pair<string, unsigned long> entry : trigramEntry.second)
      total += entry.second;
    map<string, double> frequencyEntry;
    for (pair<string, unsigned long> entry : trigramEntry.second)
      frequencyEntry.emplace(entry.first, (double)entry.second / (double)total);
    frequencyCorpus.emplace(trigramEntry.first, frequencyEntry);
  }

  MarkovCorpus mcorpus;
  mcorpus.corpus = frequencyCorpus;
  return mcorpus;
}
