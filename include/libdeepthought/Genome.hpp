#ifndef GENOME_HPP
#define GENOME_HPP

#include <string>

class Genome
{
public:
  explicit Genome();
  explicit Genome(double B, double G, double R, double C, std::string name);
  
  double B, G, R, C;
  std::string Name;
private:
};

#endif
