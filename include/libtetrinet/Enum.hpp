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

typedef uint16_t uCoord;
typedef  int32_t sCoord;
typedef uint32_t uAxis;
typedef std::pair<PieceRotation, PieceRotation> PieceRotationPair;
typedef std::pair<sCoord, sCoord> TransformPair;
typedef std::vector<FieldElement> FieldType; // origin top left, row-major

typedef std::map<PieceRotationPair, std::array<TransformPair, 5> > SRSKickMap;

inline std::ostream& operator<<(std::ostream& os, const FieldElement& element)
{
  return os << char(element);
}

#endif
