#ifndef FIELD_HPP
#define FIELD_HPP

#include <vector>
#include <memory>

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

enum class PieceShape : uint8_t
{
  I, O, J, L, S, Z, T
};

typedef PieceRotation uint8_t;

class Piece
{
  friend Piece operator "" _pd(const char*, size_t);

public:
  const FieldTransform GetTransform(unsigned x, unsigned y) const;

private:
  Piece(PieceShape shape, PieceRotation rotation, uint16_t definition, uint8_t width, uint8_t height);
  // implicit copy constructor
  ~Piece();
  
  const PieceRotation rotation;
  const PieceShape shape;
  const uint16_t definition;
  const uint8_t width, height; // apparent size, actual is always 4x4
};

// Takes form "I2111100000000000041"
// "[shape][state][16 bit definition][width][height]"
Piece operator "" _pd(const char* definition, size_t size); // size must always be 20 (turn this off for release) TODO

typedef FieldPtr std::auto_ptr<std::vector<FieldElement> >; // origin top left, row-major

class FieldTransform
{
};

class Field : private boost::noncopyable
{
public:
  Field(unsigned width = 12, unsigned height = 22);
  ~Field();

  bool IsValidTransform(const FieldTransform& transform);
  void ApplyTransform(const FieldTransform& transform);
  
  const FieldElement operator()(unsigned x, unsigned y);
private:
  const unsigned fieldWidth, fieldHeight, fieldSize;
  FieldPtr field;
};

#endif
