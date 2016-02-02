#include "libdeepthought/GenomePtr.hpp"

using namespace Wt::Dbo;

GenomePtr::GenomePtr(const ptr<Genome>& genome)
  : ptr<Genome>(genome)
{
}
