#ifndef PIECELOCATION_HPP
#define PIECELOCATION_HPP

class PieceLocation;

#include "libtetrinet/Piece.hpp"
#include "libtetrinet/FieldTransform.hpp"

class PieceLocation
{
public:
  PieceLocation(const Piece& piece, sCoord x, sCoord y);
  PieceLocation(const Piece& piece, sCoord x, sCoord y, FieldElement element);

  sCoord GetX() const;
  sCoord GetY() const;
  const Piece& GetPiece() const;
  const FieldElement& GetElement() const;
  void SetX(sCoord x);
  void SetY(sCoord y);
  const FieldTransform& GetTransform() const;

  operator std::string() const;
  
  bool operator == (const PieceLocation& rhs) const;
  PieceLocation& operator = (const PieceLocation& rhs);
private:
  sCoord x;
  sCoord y;
  FieldTransform transform;
  Piece piece;
  FieldElement element;

  void updateTransform();
};

namespace std
{
  template<> struct hash<PieceLocation>
  {
    size_t operator() (const PieceLocation& location) const
    {
      return std::hash<std::string>()(location);
    }
  };
}

#endif
