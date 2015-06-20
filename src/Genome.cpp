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

void soci::type_conversion<Genome>::from_base(values const& v, indicator ind, Genome& g)
{
  g.id = v.get<int>("id");
  g.B = v.get<double>("B");
  g.G = v.get<double>("G");
  g.R = v.get<double>("R");
  g.C = v.get<double>("C");
}

void soci::type_conversion<Genome>::to_base(const Genome& g, values& v, indicator& ind)
{
  v.set("id", g.id);
  v.set("B", g.B);
  v.set("G", g.G);
  v.set("R", g.R);
  v.set("C", g.C);
}
