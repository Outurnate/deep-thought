#ifndef GENERATION_HPP
#define GENERATION_HPP

#include <map>
#include <string>
#include <vector>
#include <soci/soci.h>

#include "Genome.hpp"
#include "Match.hpp"

class Generation;

namespace soci
{
  template<>
  struct soci::type_conversion<Generation>;
}

class Generation
{
  friend struct soci::type_conversion<Generation>;
  
public:
  explicit Generation();
  virtual ~Generation();
  
  /**
   * Retrieve a list of loaded genomes in this generation
   */
  const std::map<std::string, Genome>* GetGenomes() const;
  /**
   * Creates matches for training
   */
  std::vector<Match> GetTrainingMatches() const;
private:
  /** database id for ORM */ int id;
  std::map<std::string, Genome>* genomes;
};

namespace soci
{
  template<>
  struct type_conversion<Generation>
  {
    typedef values base_type;

    static void from_base(values const& v, indicator ind, Generation& g)
    {
      g.id = v.get<int>("id");
    }
    
    static void to_base(const Generation &g, values &v, indicator &ind)
    {
      v.set("id", g.id);
    }
  };
}


#endif
