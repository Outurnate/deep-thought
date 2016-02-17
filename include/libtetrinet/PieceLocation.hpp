#ifndef PIECELOCATION_HPP
#define PIECELOCATION_HPP

#include "libtetrinet/TetrinetForward.hpp"
#include "libtetrinet/FieldTransform.hpp"
#include "libtetrinet/Piece.hpp"

class PieceLocation : public FieldTransform
{
public:
  PieceLocation(const Piece& piece, sCoord x, sCoord y);
  PieceLocation(const Piece& piece, sCoord x, sCoord y, FieldElement element);
  PieceLocation(const PieceLocation& location);

  sCoord GetX() const;
  sCoord GetY() const;
  const Piece& GetPiece() const;
  const FieldElement& GetElement() const;
  void SetX(sCoord x);
  void SetY(sCoord y);

  operator std::string() const;
  
  //bool operator == (const PieceLocation& rhs) const;
  PieceLocation& operator = (const PieceLocation& rhs);
private:
  sCoord x;
  sCoord y;
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
