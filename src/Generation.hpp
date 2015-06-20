#ifndef GENERATION_HPP
#define GENERATION_HPP

#include <string>
#include <vector>
#include <soci/soci.h>

#include "Genome.hpp"
#include "Match.hpp"

class Generation;

namespace soci
{
  template<>
  struct type_conversion<Generation>
  {
    typedef values base_type;

    static void from_base(values const& v, indicator ind, Generation& g);
    static void to_base(const Generation& g, values& v, indicator& ind);
  };
}

class Generation
{
  friend struct soci::type_conversion<Generation>;
  friend class Population; // commit
  
public:
  explicit Generation(std::string connectionString);
  virtual ~Generation();
  
  /**
   * Retrieve a list of loaded genomes in this generation
   */
  const std::vector<Genome>* GetGenomes() const;
  /**
   * Creates matches for training
   */
  std::vector<Match> GetTrainingMatches() const;

  /** Size of the generation */
  static const unsigned Size = 100;
private:
  void commit() const;
  
  /** database id for ORM */ int id;
  std::vector<Genome>* genomes; // TODO fixed size
  std::string connectionString;
  soci::session* sql;
};

#endif
