#ifndef POPULATIONPTR_HPP
#define POPULATIONPTR_HPP

#include <Wt/Dbo/Dbo>

#include "DeepThoughtForward.hpp"

class PopulationPtr : private Wt::Dbo::ptr<Population>
{
  friend class GenerationPtr;

  typedef std::vector<GenerationPtr>::iterator iterator;
  typedef std::vector<GenerationPtr>::const_iterator const_iterator;
public:
  PopulationPtr(AIManager& manager, const std::string& name);
  PopulationPtr(const Wt::Dbo::ptr<Population>& population);

  const std::string& GetName() const;
  void SetName(const std::string& name);

  size_t GenerationCount() const;

  iterator begin();
  iterator end();

  const_iterator cbegin() const;
  const_iterator cend() const;
private:
  void updateGenerations() const;
  
  mutable boost::optional<std::vector<GenerationPtr> > generationPtrs;
};

#endif
