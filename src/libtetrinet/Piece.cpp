#include "libtetrinet/Piece.hpp"

#include <assert.h>
#include <string>
#include <iostream>

#include "libtetrinet/Enum.hpp"

using namespace std;

Piece Piece::Get(PieceShape shape, PieceRotation rotation)
{
  return defs[shape][uint8_t(rotation)];
}

bool Piece::operator() (uCoord x, uCoord y) const
{
  if (x >= GetWidth())
    throw out_of_range("x");
  if (y >= GetHeight())
    throw out_of_range("y");
  return definition[(y * pieceWidth) + x];
}

uAxis Piece::GetWidth() const
{
  return width;
}

uAxis Piece::GetHeight() const
{
  return height;
}

ostream& operator << (ostream& os, const Piece& piece)
{
  return os << piece.definition.to_string();
}

Piece& Piece::operator = (const Piece& piece)
{
  if(this == &piece)
    return *this;

  this->rotation   = piece.rotation;
  this->shape      = piece.shape;
  this->definition = piece.definition;
  this->width      = piece.width;
  this->height     = piece.height;
  
  return *this;
}

constexpr Piece::Piece(PieceShape shape, PieceRotation rotation, PieceDefinition definition, uCoord width, uCoord height)
  : rotation(rotation), shape(shape), definition(definition), width(width), height(height)
{
}

bool Piece::Rotate(const Field& field, RotationDirection direction, sCoord& x, sCoord& y)
{
  if (this->shape == PieceShape::O)
    return true;
  
  PieceRotation rotation = PieceRotation(uint8_t(this->rotation) + (bool(direction) ? -1 : 1));
  Piece newPiece = Get(this->shape, rotation);
  for (TransformPair& transform : (this->shape == PieceShape::I ? srsmap_i : srsmap_jlstz)[PieceRotationPair(this->rotation, rotation)])
  {
    try
    {
      FieldTransform ftransform(field, newPiece, x + transform.first, y + transform.second, FieldElement::RED);
      if (all_of(ftransform.begin(), ftransform.end(), [&field](pair<uCoord, FieldElement> element)
		 {
		   return element.second != FieldElement::NONE && field(element.first) == FieldElement::NONE;
		 }))
      {
	*this = newPiece;
	x += transform.first;
	y += transform.second;
	return true;
      }
    }
    catch (std::out_of_range)
    {
      continue; // piece was placed off field
    }
  }
  return false;
}

// here be dragons
constexpr bool isNumeric(char c)
{
  return c >= '0' && c <= '9';
}

constexpr unsigned _stoi(const char* string, unsigned base, unsigned count, unsigned val = 0)
{
  return count != 0 ?
    isNumeric(*string) ?
      _stoi(string - 1, base, count - 1, (*string - '0' + val * base)) : throw "invalid numeric string"
    : val;
}

constexpr unsigned _bc(const char* string, unsigned base = 10, unsigned count = 0)
{
  return *string ? // string null or not
    _bc(string + 1, base, count + 1) // recurse, counting
    : _stoi(string - 1, base, count); // at the end of the string, trigger loop back
}

constexpr unsigned stoi(const char* string, unsigned base = 10)
{
  return _bc(string, base);
}

template <typename T>
constexpr T numCharToNum(char num) // digit to number
{
//  assert(num >= '0' && num <= '9');
  return T(num - '0');
}
// end dragons

constexpr Piece operator "" _pd(const char* definition, size_t size)
{
//  assert(size == 20);
  return Piece(static_cast<PieceShape>(definition[0]),
	       numCharToNum<PieceRotation>(definition[1]),
	       Piece::PieceDefinition(stoi(&definition[4], 2)),
	       numCharToNum<uCoord>(definition[2]),
	       numCharToNum<uCoord>(definition[3]));
}

SRSKickMap Piece::srsmap_jlstz = SRSKickMap
{
  {
    PieceRotationPair(PieceRotation::Z, PieceRotation::R),
    {
      TransformPair(  0,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( -1,  +1 ),
      TransformPair(  0,  -2 ),
      TransformPair( -1,  -2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::R, PieceRotation::Z),
    {
      TransformPair(  0,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( +1,  -1 ),
      TransformPair(  0,  +2 ),
      TransformPair( +1,  +2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::R, PieceRotation::T),
    {
      TransformPair(  0,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( +1,  -1 ),
      TransformPair(  0,  +2 ),
      TransformPair( +1,  +2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::T, PieceRotation::R),
    {
      TransformPair(  0,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( -1,  +1 ),
      TransformPair(  0,  -2 ),
      TransformPair( -1,  -2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::T, PieceRotation::L),
    {
      TransformPair(  0,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( +1,  +1 ),
      TransformPair(  0,  -2 ),
      TransformPair( +1,  -2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::L, PieceRotation::T),
    {
      TransformPair(  0,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( -1,  -1 ),
      TransformPair(  0,  +2 ),
      TransformPair( -1,  +2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::L, PieceRotation::Z),
    {
      TransformPair(  0,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( -1,  -1 ),
      TransformPair(  0,  +2 ),
      TransformPair( -1,  +2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::Z, PieceRotation::L),
    {
      TransformPair(  0,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( +1,  +1 ),
      TransformPair(  0,  -2 ),
      TransformPair( +1,  -2 )
    }
  },
};

SRSKickMap Piece::srsmap_i = SRSKickMap
{
  {
    PieceRotationPair(PieceRotation::Z, PieceRotation::R),
    {
      TransformPair(  0,  0  ),
      TransformPair( -2,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( -2,  -1 ),
      TransformPair( +1,  +2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::R, PieceRotation::Z),
    {
      TransformPair(  0,  0  ),
      TransformPair( +2,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( +2,  +1 ),
      TransformPair( -1,  -2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::R, PieceRotation::T),
    {
      TransformPair(  0,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( +2,  0  ),
      TransformPair( -1,  +2 ),
      TransformPair( +2,  -1 )
    }
  },
  {
    PieceRotationPair(PieceRotation::T, PieceRotation::R),
    {
      TransformPair(  0,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( -2,  0  ),
      TransformPair( +1,  -2 ),
      TransformPair( -2,  +1 )
    }
  },
  {
    PieceRotationPair(PieceRotation::T, PieceRotation::L),
    {
      TransformPair(  0,  0  ),
      TransformPair( +2,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( +2,  +1 ),
      TransformPair( -1,  -2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::L, PieceRotation::T),
    {
      TransformPair(  0,  0  ),
      TransformPair( -2,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( -2,  -1 ),
      TransformPair( +1,  +2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::L, PieceRotation::Z),
    {
      TransformPair(  0,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( -2,  0  ),
      TransformPair( +1,  -2 ),
      TransformPair( -2,  +1 )
    }
  },
  {
    PieceRotationPair(PieceRotation::Z, PieceRotation::L),
    {
      TransformPair(  0,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( +2,  0  ),
      TransformPair( -1,  +2 ),
      TransformPair( +2,  -1 )
    }
  },
};

PieceDefinitionMap Piece::defs = PieceDefinitionMap
{
  {
    PieceShape::I,
    {
      "I044"
        "0000"
        "1111"
        "0000"
        "0000"_pd,
      "I144"
        "0010"
        "0010"
        "0010"
        "0010"_pd,
      "I244"
        "0000"
        "0000"
        "1111"
        "0000"_pd,
      "I344"
        "0100"
        "0100"
        "0100"
        "0100"_pd,
    }
  },
  {
    PieceShape::O,
    {
      "O022"
        "1100"
        "1100"
        "0000"
        "0000"_pd
    }
  },
  {
    PieceShape::J,
    {
      "J033"
        "1000"
        "1110"
        "0000"
        "0000"_pd,
      "J133"
        "0110"
        "0100"
        "0100"
        "0000"_pd,
      "J233"
        "0000"
        "1110"
        "0010"
        "0000"_pd,
      "J333"
        "0100"
        "0100"
        "1100"
        "0000"_pd
    }
  },
  {
    PieceShape::L,
    {
      "L033"
        "0010"
        "1110"
        "0000"
        "0000"_pd,
      "L133"
        "0100"
        "0100"
        "0110"
        "0000"_pd,
      "L233"
        "0000"
        "1110"
        "1000"
        "0000"_pd,
      "L332"
        "1100"
        "0100"
        "0100"
        "0000"_pd
    }
  },
  {
    PieceShape::S,
    {
      "S033"
        "0110"
        "1100"
        "0000"
        "0000"_pd,
      "S133"
        "0100"
        "0110"
        "0010"
        "0000"_pd,
      "S233"
        "0000"
        "0110"
        "1100"
        "0000"_pd,
      "S333"
        "1000"
        "1100"
        "0100"
        "0000"_pd
    }
  },
  {
    PieceShape::Z,
    {
      "Z033"
        "0110"
        "1100"
        "0000"
        "0000"_pd,
      "Z133"
        "0100"
        "0110"
        "0010"
        "0000"_pd,
      "Z233"
        "0000"
        "0110"
        "1100"
        "0000"_pd,
      "Z333"
        "1000"
        "1100"
        "0100"
        "0000"_pd
    }
  },
  {
    PieceShape::T,
    {
      "T033"
        "0100"
        "1110"
        "0000"
        "0000"_pd,
      "T133"
        "0100"
        "0110"
        "0100"
        "0000"_pd,
      "T233"
        "0000"
        "1110"
        "0100"
        "0000"_pd,
      "T333"
        "0100"
        "1100"
        "0100"
        "0000"_pd
    }
  }
};
