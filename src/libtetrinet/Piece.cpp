#include "libtetrinet/Piece.hpp"

#include <assert.h>
#include <string>
#include <iostream>

#include "libtetrinet/Enum.hpp"

using namespace std;
using namespace boost;

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

PieceRotation Piece::GetRotation() const
{
  return rotation;
}

PieceShape Piece::GetShape() const
{
  return shape;
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

Piece::operator std::string() const
{
  return lexical_cast<std::string>(int(rotation)) + lexical_cast<std::string>(int(shape)) + definition.to_string()
    + lexical_cast<std::string>(width) + lexical_cast<std::string>(height);
}

bool Piece::operator == (const Piece& rhs) const
{
  return rotation == rhs.rotation && shape == rhs.shape && definition == rhs.definition && width == rhs.width && height == rhs.height;
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
