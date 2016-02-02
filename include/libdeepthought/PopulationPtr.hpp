#ifndef POPULATIONPTR_HPP
#define POPULATIONPTR_HPP

#include <Wt/Dbo/Dbo>

#include "DeepThoughtForward.hpp"

class PopulationPtr : private Wt::Dbo::ptr<Population>
{
  friend class GenerationPtr;
public:
  PopulationPtr(AIManager& manager, const std::string& name, int generationSize, int elitists, int crossovers, int mutants);
  PopulationPtr(const Wt::Dbo::ptr<Population>& population);

  const std::string& GetName() const;
  void SetName(const std::string& name);

  size_t GenerationCount() const;
  const std::unique_ptr<GenerationPtr> GetCurrentGeneration() const;
};

#endif
