#include "libtetrinet/FieldElement.hpp"

#include <map>
#include <array>
#include <ostream>

#include "libtetrinet/PieceShape.hpp"

using namespace std;

FieldElement GetColor(const PieceShape& shape)
{
  static map<PieceShape, array<FieldElement, 3> > map =
    {
      { PieceShape::I, { FieldElement::BLUE,   FieldElement::YELLOW, FieldElement::GREEN  } },
      { PieceShape::O, { FieldElement::YELLOW, FieldElement::GREEN,  FieldElement::PURPLE } },
      { PieceShape::T, { FieldElement::GREEN,  FieldElement::PURPLE, FieldElement::RED    } },
      { PieceShape::S, { FieldElement::RED,    FieldElement::BLUE,   FieldElement::YELLOW } },
      { PieceShape::Z, { FieldElement::BLUE,   FieldElement::GREEN,  FieldElement::RED    } },
      { PieceShape::J, { FieldElement::BLUE,   FieldElement::YELLOW, FieldElement::PURPLE } },
      { PieceShape::L, { FieldElement::BLUE,   FieldElement::PURPLE, FieldElement::RED    } }
    };
  return map[shape][0];
}

ostream& operator<<(ostream& os, const FieldElement& element)
{
  return os << static_cast<char>(element);
}
