#include "Piece.hpp"

#include <assert.h>
#include <string>

#include "Enum.hpp"

using namespace std;

const FieldTransform Piece::GetTransform(unsigned x, unsigned y, FieldElement element) const
{
  FieldTransform transform;
  for (uint8_t i = 0; i < (width * height); ++i)
  {
    uint8_t x2 = i % width,
            y2 = i / width;
    if ((*this)(x2, y2))
      transform(x + x2, y + y2) = element;
  }
  return transform;
}

Piece Piece::Get(PieceShape shape, PieceRotation rotation)
{
  return defs[shape][rotation];
}

bool Piece::operator() (unsigned x, unsigned y) const
{
  if (x >= width)
    throw out_of_range("x");
  if (y >= height)
    throw out_of_range("y");
  return definition[(y * pieceWidth) + x];
}

unsigned Piece::GetWidth() const
{
  return width;
}

unsigned Piece::GetHeight() const
{
  return height;
}

ostream& operator << (ostream& os, const Piece& piece)
{
  return os << piece.definition.to_string();
}

constexpr Piece::Piece(PieceShape shape, PieceRotation rotation, PieceDefinition definition, uint8_t width, uint8_t height)
  : rotation(rotation), shape(shape), definition(definition), width(width), height(height)
{
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
  return num - '0';
}
// end dragons

constexpr Piece operator "" _pd(const char* definition, size_t size)
{
//  assert(size == 20);
  return Piece(static_cast<PieceShape>(definition[0]),
	       numCharToNum<PieceRotation>(definition[1]),
	       Piece::PieceDefinition(stoi(&definition[4], 2)),
	       numCharToNum<uint8_t>(definition[2]),
	       numCharToNum<uint8_t>(definition[3]));
}

PieceDefinitionMap Piece::defs = PieceDefinitionMap
{
  {
    PieceShape::I,
    {
      "I041"
        "1111"
        "0000"
        "0000"
        "0000"_pd,
      "I114"
        "1000"
        "1000"
        "1000"
        "1000"_pd,
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
      "J023"
        "0100"
        "0100"
        "1100"
        "0000"_pd,
      "J132"
        "1000"
        "1110"
        "0000"
        "0000"_pd,
      "J232"
        "1100"
        "1000"
        "1000"
        "0000"_pd,
      "J332"
        "1110"
        "0010"
        "0000"
        "0000"_pd
    }
  },
  {
    PieceShape::L,
    {
      "L023"
        "1000"
        "1000"
        "1100"
        "0000"_pd,
      "L132"
        "0010"
        "1110"
        "0000"
        "0000"_pd,
      "L223"
        "1100"
        "0100"
        "0100"
        "0000"_pd,
      "L332"
        "1110"
        "1000"
        "0000"
        "0000"_pd
    }
  },
  {
    PieceShape::S,
    {
      "S032"
        "0110"
        "1100"
        "0000"
        "0000"_pd,
      "S123"
        "1000"
        "1100"
        "0100"
        "0000"_pd
    }
  },
  {
    PieceShape::Z,
    {
      "Z032"
        "1100"
        "0110"
        "0000"
        "0000"_pd,
      "Z123"
        "0100"
        "1100"
        "1000"
        "0000"_pd
    }
  },
  {
    PieceShape::T,
    {
      "T032"
        "1110"
        "0100"
        "0000"
        "0000"_pd,
      "T123"
        "1000"
        "1100"
        "1000"
        "0000"_pd,
      "T232"
        "0100"
        "1110"
        "0000"
        "0000"_pd,
      "T323"
        "0100"
        "1100"
        "0100"
        "0000"_pd
    }
  }
};
