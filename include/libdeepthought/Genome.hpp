#ifndef GENOME_HPP
#define GENOME_HPP

#include <Wt/Dbo/Dbo>
#include <string>

#include "DeepThoughtForward.hpp"

#include "Generation.hpp"
#include "Match.hpp"

class Genome
{
public:
  const double MAXINT = std::numeric_limits<std::mt19937::result_type>::max();
  const double MING = -20.0;
  const double MAXG = 20.0;
  const double RANGEG = MAXG - MING;
  
  Genome() = default;
  Genome(const Genome& genomeA, const Genome& genomeB)
    : B((genomeA.B + genomeB.B) / 2.0),
      G((genomeA.G + genomeB.G) / 2.0),
      R((genomeA.R + genomeB.R) / 2.0),
      C((genomeA.C + genomeB.C) / 2.0) {}
  Genome(const Genome& genome, std::mt19937& g)
    : B((g() / MAXINT) < 0.5 ? genome.B : ((g() / MAXINT) * RANGEG) + MING),
      G((g() / MAXINT) < 0.5 ? genome.G : ((g() / MAXINT) * RANGEG) + MING),
      R((g() / MAXINT) < 0.5 ? genome.R : ((g() / MAXINT) * RANGEG) + MING),
      C((g() / MAXINT) < 0.5 ? genome.C : ((g() / MAXINT) * RANGEG) + MING) {}
  Genome(std::mt19937& g)
    : B(((g() / MAXINT) * RANGEG) + MING),
      G(((g() / MAXINT) * RANGEG) + MING),
      R(((g() / MAXINT) * RANGEG) + MING),
      C(((g() / MAXINT) * RANGEG) + MING) {}
  
  template<typename Action>
  void persist(Action& a)
  {
    Wt::Dbo::field(a, Name, "name");
    Wt::Dbo::field(a, B, "b");
    Wt::Dbo::field(a, G, "g");
    Wt::Dbo::field(a, R, "r");
    Wt::Dbo::field(a, C, "c");
    Wt::Dbo::hasMany(a, owners, Wt::Dbo::ManyToMany, "generation_genomes");
    Wt::Dbo::hasMany(a, matches, Wt::Dbo::ManyToOne, "genome");
  }

  // DBO fields
  Wt::Dbo::collection<Wt::Dbo::ptr<Generation> > owners;
  Wt::Dbo::collection<Wt::Dbo::ptr<Match> > matches;
  double B, G, R, C;
  std::string Name;
};

#endif
