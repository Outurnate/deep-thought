#ifndef PIECE_HPP
#define PIECE_HPP

#include <stdint.h>
#include <vector>
#include <map>
#include <bitset>
#include <iostream>

class Piece;

#include "libtetrinet/Field.hpp"
#include "libtetrinet/Enum.hpp"

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
  Piece& operator= (const Piece& piece);
  
  Coord GetWidth() const;
  Coord GetHeight() const;

  bool Rotate(const Field& field, RotationDirection direction, Coord& x, Coord& y);

  static Piece Get(PieceShape shape, PieceRotation rotation);
private:
  static const Coord pieceWidth = 4, pieceHeight = 4, pieceSize = pieceWidth * pieceHeight;
  typedef std::bitset<pieceSize> PieceDefinition;
  
  constexpr Piece(PieceShape shape, PieceRotation rotation, PieceDefinition definition, Coord width, Coord height);
  
  PieceRotation rotation;
  PieceShape shape;
  PieceDefinition definition;
  Coord width, height; // apparent size, actual is always 4x4
  
  static PieceDefinitionMap defs;
  static SRSKickMap srsmap_jlstz, srsmap_i;
};

#endif
