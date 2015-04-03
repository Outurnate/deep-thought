#ifndef GENOME_HPP
#define GENOME_HPP

#include <string>

class Genome
{
public:
  Genome();
  Genome(double B, double G, double R, double C, std::string name);
  virtual ~Genome();
  
  double B, G, R, C;
  std::string Name;
};

#endif
