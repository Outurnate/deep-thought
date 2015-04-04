#include <map>
#include <string>

using namespace std;

void main()
{
  // A - trigram
  // B - single char next
  // C - total
      /*AA*/      /*BB*/  /*CCCCCCCCC*/
  map<string, map<string, unsigned long>> corpus;

  // Each line will contain one word
  for (string line; getline(cin, line);)
    for (unsigned i = 0; i < (line.length - 4); ++i)
    {
      string trigram = line.substr(i, 3);
      string next = line.substr(i + 3, 0);

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
      frequencyEntry.emplace(entry.first, entry.second / total);
    frequencyCorpus.emplace(trigramEntry.first, frequencyEntry);
  }
}
