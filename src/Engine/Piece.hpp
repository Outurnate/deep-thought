#ifndef PIECE_HPP
#define PIECE_HPP

#include <stdint.h>
#include <vector>
#include <map>
#include <boost/mpl/map.hpp>

#include "Enum.hpp"
#include "FieldTransform.hpp"

class Piece;

typedef std::map<PieceShape, std::vector<Piece> > PieceDefinitionMap;

constexpr Piece operator "" _pd(const char* definition, size_t size);

class Piece
{
  // Takes form "I2411111000000000000"
  // "[shape][state][width][height][16 bit definition]"
  constexpr Piece friend operator "" _pd(const char* definition, size_t size); // size must always be 20 (turn this off for release) TODO

public:
  const FieldTransform GetTransform(unsigned x, unsigned y) const;

  static Piece Get(PieceShape shape, PieceRotation rotation);
private:
  constexpr Piece(PieceShape shape, PieceRotation rotation, uint16_t definition, uint8_t width, uint8_t height);
  
  const PieceRotation rotation;
  const PieceShape shape;
  const uint16_t definition;
  const uint8_t width, height; // apparent size, actual is always 4x4

  static PieceDefinitionMap defs;
};

#endif
