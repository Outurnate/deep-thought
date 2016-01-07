#ifndef GENERATION_HPP
#define GENERATION_HPP

#include <string>
#include <vector>

#include "Genome.hpp"
#include "Match.hpp"

class Generation
{
public:
  explicit Generation();
  
  /**
   * Retrieve a list of loaded genomes in this generation
   */
  const std::shared_ptr<std::vector<Genome>> GetGenomes() const;
  /**
   * Creates matches for training
   */
  std::vector<Match> GetTrainingMatches() const;
private:
  std::shared_ptr<std::vector<Genome>> genomes;
};

#endif
