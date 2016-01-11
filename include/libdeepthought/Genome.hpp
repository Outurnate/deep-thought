#ifndef GENOME_HPP
#define GENOME_HPP

#include <Wt/Dbo/Dbo>
#include <string>

#include "DeepThoughtForward.hpp"

class Genome
{
public:
  explicit Genome();
  explicit Genome(double B, double G, double R, double C, std::string name);
  
  double B, G, R, C;
  std::string Name;

  template<typename Action>
  void persist(Action& a)
  {
    Wt::Dbo::field(a, Name, "name");
    Wt::Dbo::field(a, B, "b");
    Wt::Dbo::field(a, G, "g");
    Wt::Dbo::field(a, R, "r");
    Wt::Dbo::field(a, C, "c");
    Wt::Dbo::belongsTo(a, owner, "generation");
  }
private:
  Wt::Dbo::ptr<Generation> owner;
};

#endif
