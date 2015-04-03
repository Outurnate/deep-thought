#ifndef GENERATION_HPP
#define GENERATION_HPP

#include <map>
#include <string>
#include <vector>

#include "Genome.hpp"
#include "Match.hpp"

class Generation
{
public:
  Generation(int genomes);
  virtual ~Generation();
  
  /**
   * Retrieve a list of loaded genomes in this generation
   */
  const std::map<std::string, Genome>* GetGenomes() const; // TODO maybe back to set?
  /**
   * Creates matches for training
   */
  std::vector<Match> GetTrainingMatches();
private:
  std::map<std::string, Genome>* genomes;
  std::vector<AIEngine*>* engines;
};

#endif
