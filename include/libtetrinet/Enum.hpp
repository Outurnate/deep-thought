#ifndef ENUM_HPP
#define ENUM_HPP

#include <stdint.h>
#include <vector>
#include <iostream>
#include <map>
#include <array>

unsigned long constexpr djb2(const char* c);

unsigned long constexpr djb2(const char* c, unsigned long hash)
{
  return !*c ? hash : djb2(c + 1, ((hash << 5) + hash) + *c);
}

unsigned long constexpr djb2(const char* c)
{
  return djb2(c, 5381);
}

enum class SpecialPiece : char
{
  ADDLINE       = 'a',
  CLEARLINE     = 'c',
  NUKE          = 'n',
  RANDOMCLEAR   = 'r',
  SWITCH        = 's',
  CLEARSPECIALS = 'b',
  GRAVITY       = 'g',
  QUAKE         = 'q',
  BOMB          = 'o'
};

const std::vector<SpecialPiece> AllSpecialPiece
{
  SpecialPiece::ADDLINE,
  SpecialPiece::CLEARLINE,
  SpecialPiece::NUKE,
  SpecialPiece::RANDOMCLEAR,
  SpecialPiece::SWITCH,
  SpecialPiece::CLEARSPECIALS,
  SpecialPiece::GRAVITY,
  SpecialPiece::QUAKE,
  SpecialPiece::BOMB,
};

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

const std::vector<PieceShape> AllPieceShape
{
  PieceShape::I,
  PieceShape::O,
  PieceShape::J,
  PieceShape::L,
  PieceShape::S,
  PieceShape::Z,
  PieceShape::T
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

unsigned long constexpr
  TOKEN_F            = djb2("f"           ),
  TOKEN_SB           = djb2("sb"          ),
  TOKEN_LVL          = djb2("lvl"         ),
  TOKEN_GMSG         = djb2("gmsg"        ),
  TOKEN_TEAM         = djb2("team"        ),
  TOKEN_PLINE        = djb2("pline"       ),
  TOKEN_PAUSE        = djb2("pause"       ),
  TOKEN_INGAME       = djb2("ingame"      ),
  TOKEN_NEWGAME      = djb2("newgame"     ),
  TOKEN_ENDGAME      = djb2("endgame"     ),
  TOKEN_WINLIST      = djb2("winlist"     ),
  TOKEN_PLINEACT     = djb2("plineact"    ),
  TOKEN_PLAYERWON    = djb2("playerwon"   ),
  TOKEN_PLAYERNUM    = djb2("playernum"   ),
  TOKEN_PLAYERLOST   = djb2("playerlost"  ),
  TOKEN_PLAYERJOIN   = djb2("playerjoin"  ),
  TOKEN_PLAYERLEAVE  = djb2("playerleave" ),
  TOKEN_NOCONNECTING = djb2("noconnecting");

enum class TetrinetMessage : unsigned long
{
  F            = TOKEN_F,
  SB           = TOKEN_SB,
  LVL          = TOKEN_LVL,
  GMSG         = TOKEN_GMSG,
  TEAM         = TOKEN_TEAM,
  PLINE        = TOKEN_PLINE,
  PAUSE        = TOKEN_PAUSE,
  INGAME       = TOKEN_INGAME,
  NEWGAME      = TOKEN_NEWGAME,
  ENDGAME      = TOKEN_ENDGAME,
  WINLIST      = TOKEN_WINLIST,
  PLINEACT     = TOKEN_PLINEACT,
  PLAYERWON    = TOKEN_PLAYERWON,
  PLAYERNUM    = TOKEN_PLAYERNUM,
  PLAYERLOST   = TOKEN_PLAYERLOST,
  PLAYERJOIN   = TOKEN_PLAYERJOIN,
  PLAYERLEAVE  = TOKEN_PLAYERLEAVE,
  NOCONNECTING = TOKEN_NOCONNECTING
};

const std::map<TetrinetMessage, std::string> MessageMap =
{
  { TetrinetMessage::F,            "f"            },
  { TetrinetMessage::SB,           "sb"           },
  { TetrinetMessage::LVL,          "lvl",         },
  { TetrinetMessage::GMSG,         "gmsg"         },
  { TetrinetMessage::TEAM,         "team"         },
  { TetrinetMessage::PLINE,        "pline"        },
  { TetrinetMessage::PAUSE,        "pause"        },
  { TetrinetMessage::INGAME,       "ingame"       },
  { TetrinetMessage::NEWGAME,      "newgame"      },
  { TetrinetMessage::ENDGAME,      "endgame"      },
  { TetrinetMessage::WINLIST,      "winlist"      },
  { TetrinetMessage::PLINEACT,     "plineact"     },
  { TetrinetMessage::PLAYERWON,    "playerwon"    },
  { TetrinetMessage::PLAYERNUM,    "playernum"    },
  { TetrinetMessage::PLAYERLOST,   "playerlost"   },
  { TetrinetMessage::PLAYERJOIN,   "playerjoin"   },
  { TetrinetMessage::PLAYERLEAVE,  "playerleave"  },
  { TetrinetMessage::NOCONNECTING, "noconnecting" }
};
//GMSG,SB,PAUSE

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
