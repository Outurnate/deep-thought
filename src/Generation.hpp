#ifndef GENERATION_HPP
#define GENERATION_HPP

#include <set>

#include "Genome.hpp"

class Generation
{
  /**
   * Retrieve a list of loaded genomes in this generation
   */
  const std::set<std::string, Genome>* GetGenomes() const;
};

#endif
