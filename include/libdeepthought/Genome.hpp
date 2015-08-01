#ifndef GENOME_HPP
#define GENOME_HPP

#include <string>
#include <soci/soci.h>

class Genome;
class Generation;

namespace soci
{
  template<>
  struct type_conversion<Genome>
  {
    typedef values base_type;

    static void from_base(values const& v, indicator ind, Genome& g);
    static void to_base(const Genome& g, values& v, indicator& ind);
  };
}

class Genome
{
  friend struct soci::type_conversion<Generation>; // commit
  friend struct soci::type_conversion<Genome>;
  
public:
  explicit Genome();
  explicit Genome(double B, double G, double R, double C, std::string name);
  virtual ~Genome();
  
  double B, G, R, C;
  std::string Name;
private:
  /** database id for ORM */ int id;
};

#endif
