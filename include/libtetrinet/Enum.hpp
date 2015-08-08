#ifndef ENUM_HPP
#define ENUM_HPP

#include <stdint.h>
#include <vector>
#include <iostream>
#include <map>
#include <array>

enum class FieldElement : char
{
  BLUE          = '1',
  YELLOW        = '2',
  GREEN         = '3',
  PURPLE        = '4',
  RED           = '5',

  NONE          = '0',

  ADDLINE       = 'a',
  CLEARLINE     = 'c',
  NUKE          = 'n',
  RANDOMCLEAR   = 'r',
  SWITCH        = 's',
  CLEARSPECIALS = 'b',
  GRAVITY       = 'g',
  QUAKE         = 'q',
  BOMB          = 'o',

  UNDEFINED     = '-' // used to specify ignorance in transform
};

enum class PieceShape : char
{
  I = 'I',
  O = 'O',
  J = 'J',
  L = 'L',
  S = 'S',
  Z = 'Z',
  T = 'T'
};

enum class PieceRotation : uint8_t
{
  Z = 0, // Zero
  R = 1, // Right
  T = 2, // Twice
  L = 3  // Left
};

enum class RotationDirection : bool
{
  CW = false,
  CCW = true
};

class ColorTuple
{
public:
  ColorTuple(FieldElement a, FieldElement b, FieldElement c) : colors({ a, b, c }) {}
  FieldElement operator () () const { return colors[0];  }
private:
  const std::array<FieldElement, 3> colors;
};

typedef uint16_t uCoord;
typedef  int32_t sCoord;
typedef uint32_t uAxis;
typedef std::pair<PieceRotation, PieceRotation> PieceRotationPair;
typedef std::pair<sCoord, sCoord> TransformPair;
typedef std::vector<FieldElement> FieldType; // origin top left, row-major
typedef std::map<const PieceShape, const ColorTuple> ColorMap;

typedef std::map<PieceRotationPair, std::array<TransformPair, 5> > SRSKickMap;

inline std::ostream& operator<<(std::ostream& os, const FieldElement& element)
{
  return os << char(element);
}

inline FieldElement GetColor(PieceShape shape)
{
  switch (shape)
  {
  case PieceShape::I:
    return ColorTuple(FieldElement::BLUE,   FieldElement::YELLOW, FieldElement::GREEN)();
  case PieceShape::O:
    return ColorTuple(FieldElement::YELLOW, FieldElement::GREEN,  FieldElement::PURPLE)();
  case PieceShape::T:
    return ColorTuple(FieldElement::GREEN,  FieldElement::PURPLE, FieldElement::RED)();
  case PieceShape::S:
    return ColorTuple(FieldElement::RED,    FieldElement::BLUE,   FieldElement::YELLOW)();
  case PieceShape::Z:
    return ColorTuple(FieldElement::BLUE,   FieldElement::GREEN,  FieldElement::RED)();
  case PieceShape::J:
    return ColorTuple(FieldElement::BLUE,   FieldElement::YELLOW, FieldElement::PURPLE)();
  case PieceShape::L:
    return ColorTuple(FieldElement::BLUE,   FieldElement::PURPLE, FieldElement::RED)();
  }
}

static SRSKickMap srsmap_jlstz = SRSKickMap
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
}, srsmap_i = SRSKickMap
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

#endif
