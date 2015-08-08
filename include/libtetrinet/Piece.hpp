#ifndef PIECE_HPP
#define PIECE_HPP

#include <stdint.h>
#include <vector>
#include <map>
#include <bitset>
#include <iostream>
#include <boost/lexical_cast.hpp>

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

  friend std::ostream& operator << (std::ostream& stream, const Piece& piece);
public:
  bool operator() (uCoord x, uCoord y) const;
  Piece& operator= (const Piece& piece);
  
  uAxis GetWidth() const;
  uAxis GetHeight() const;
  PieceRotation GetRotation() const;
  PieceShape GetShape() const;

  static Piece Get(PieceShape shape, PieceRotation rotation);
  
  operator std::string() const;
  bool operator == (const Piece& rhs) const;
private:
  static const uAxis pieceWidth = 4, pieceHeight = 4, pieceSize = pieceWidth * pieceHeight;
  typedef std::bitset<pieceSize> PieceDefinition;
  
  constexpr Piece(PieceShape shape, PieceRotation rotation, PieceDefinition definition, uCoord width, uCoord height);
  
  PieceRotation rotation;
  PieceShape shape;
  PieceDefinition definition;
  uAxis width, height; // apparent size, actual is always 4x4
  
  static PieceDefinitionMap defs;
};

#endif
