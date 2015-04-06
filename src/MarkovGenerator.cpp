#include "MarkovGenerator.hpp"

#include <algorithm>
#include <iterator>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/random/discrete_distribution.hpp>
#include <boost/algorithm/string.hpp>

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

string MarkovGenerator::weightedEntry(string trigram)
{
  map<string, double> entryInit = corpus->GetCorpus()->at(trigram);
  auto beginInit = make_transform_iterator(entryInit.begin(), map_v<string, double>()),
    endInit = make_transform_iterator(entryInit.end(), map_v<string, double>());
  random::discrete_distribution<> distInit(beginInit, endInit);
  return next(entryInit.begin(), distInit(*mersenne))->first;
}

string MarkovGenerator::GenerateName()
{
  string name = weightedEntry(""); // get a random trigram from corpus
  int length = 5 + (*nameLength)(); // 5-12 length
  while (name.length() != length)
    name += weightedEntry(name.substr(name.length() - 3, 3));
  return to_upper_copy(name.substr(0, 1)) + to_lower_copy(name.substr(1, name.length() - 1));
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

void MarkovCorpus::checkEntry(_corpus& c, string trigram, string entry)
{
  // Ensure keys exist
  if (!c.count(trigram))
    c.emplace(trigram, map<string, unsigned long>());
  if (!c.at(trigram).count(entry))
    c.at(trigram).emplace(entry, 0);
}

MarkovCorpus MarkovCorpus::FromStream(istream& in)
{
  _corpus corpus;

  // Each line will contain one word
  for (string line; getline(in, line);)
  {
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    for (unsigned i = 0; i < (line.length() - 4); ++i)
    {
      string trigram = line.substr(i, 3);
      string next = line.substr(i + 3, 1);

      if (i == 0)
      {
	checkEntry(corpus, string(""), trigram);
	++corpus.at("").at(trigram);
      }

      if (next.length() == 0)
	break;

      checkEntry(corpus, trigram, next);

      // Increment
      ++corpus.at(trigram).at(next);
    }
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
