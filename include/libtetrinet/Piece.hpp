#ifndef PIECE_HPP
#define PIECE_HPP

#include <stdint.h>
#include <vector>
#include <map>
#include <bitset>
#include <iosfwd>
#include <boost/lexical_cast.hpp>
#include <experimental/optional>

#include "libtetrinet/TetrinetForward.hpp"

class Piece
{
public:
  Piece(PieceShape shape, PieceRotation rotation);
  
  bool operator== (const Piece& rhs) const;
  bool operator() (uCoord x, uCoord y) const;
  operator std::string() const;
  
  uAxis GetWidth() const;
  uAxis GetHeight() const;
  PieceRotation GetRotation() const;
  PieceShape GetShape() const;
  std::experimental::optional<uCoord> GetHeightAt(uCoord x) const;
  
  friend constexpr Piece operator "" _pd(const char* definition, size_t size);
private:
  typedef std::bitset<pieceSize> PieceDefinition;
  
  static PieceDefinitionMap defs;
  
  constexpr Piece(PieceShape shape, PieceRotation rotation, PieceDefinition definition, uCoord width, uCoord height);
  
  PieceRotation rotation;
  PieceShape shape;
  PieceDefinition definition;
  uAxis width, height; // apparent size, actual is always 4x4
  std::array<std::experimental::optional<uCoord>, pieceWidth> pieceBottoms; // x is key, y is val
};

std::ostream& operator<< (std::ostream& stream, const Piece& piece);

// Takes form "I2411111000000000000"
// "[shape][state][width][height][16 bit definition]"
constexpr Piece operator "" _pd(const char* definition, size_t size);

#endif
