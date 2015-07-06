#ifndef PIECE_HPP
#define PIECE_HPP

#include <stdint.h>
#include <vector>
#include <map>
#include <bitset>
#include <iostream>

#include "Enum.hpp"

class Piece;

typedef std::map<PieceShape, std::vector<Piece> > PieceDefinitionMap;

constexpr Piece operator "" _pd(const char* definition, size_t size);

class Piece
{
  // Takes form "I2411111000000000000"
  // "[shape][state][width][height][16 bit definition]"
  constexpr Piece friend operator "" _pd(const char* definition, size_t size); // size must always be 20 (turn this off for release) TODO

  friend std::ostream& operator<< (std::ostream& stream, const Piece& piece);
public:
  bool operator() (Coord x, Coord y) const;
  Coord GetWidth() const;
  Coord GetHeight() const;

  static Piece Get(PieceShape shape, PieceRotation rotation);
private:
  static const Coord pieceWidth = 4, pieceHeight = 4, pieceSize = pieceWidth * pieceHeight;
  typedef std::bitset<pieceSize> PieceDefinition;
  
  constexpr Piece(PieceShape shape, PieceRotation rotation, PieceDefinition definition, Coord width, Coord height);
  
  const PieceRotation rotation;
  const PieceShape shape;
  const PieceDefinition definition;
  const Coord width, height; // apparent size, actual is always 4x4
  
  static PieceDefinitionMap defs;
};

#endif
