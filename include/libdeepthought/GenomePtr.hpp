#ifndef GENOMEPTR_HPP
#define GENOMEPTR_HPP

#include <Wt/Dbo/Dbo>

#include "DeepThoughtForward.hpp"

class GenomePtr : private Wt::Dbo::ptr<Genome>
{
  friend class MatchPtr;
public:
  GenomePtr(const Wt::Dbo::ptr<Genome>& genome);
};

#endif
