#include "Genome.hpp"

Genome::Genome()
  : Genome(0.0, 0.0, 0.0, 0.0, "Kevin")
{
}

Genome::Genome(double B, double G, double R, double C, std::string name) : B(B), G(G), R(R), C(C), Name(name)
{
}

Genome::~Genome()
{
}