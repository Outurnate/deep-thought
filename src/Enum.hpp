#ifndef ENUM_HPP
#define ENUM_HPP

#include <stdint.h>
#include <vector>
#include <iostream>

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

typedef uint8_t PieceRotation;
typedef std::vector<FieldElement> FieldType; // origin top left, row-major

inline std::ostream& operator<<(std::ostream& os, const FieldElement& element)
{
  return os << char(element);
}

#endif
