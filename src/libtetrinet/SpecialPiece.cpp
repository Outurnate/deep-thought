#include "libtetrinet/SpecialPiece.hpp"

bool isSpecial(FieldElement e)
{
  return AllSpecialPiece.find(static_cast<SpecialPiece>(e)) != AllSpecialPiece.end();
}
