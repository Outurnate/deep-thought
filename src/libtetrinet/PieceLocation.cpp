#include "libtetrinet/PieceLocation.hpp"

using namespace std;
using namespace boost;

PieceLocation::PieceLocation(const Piece& piece, sCoord x, sCoord y)
  : PieceLocation(piece, x, y, GetColor(piece.GetShape())) {}

PieceLocation::PieceLocation(const Piece& piece, sCoord x, sCoord y, FieldElement element)
  : FieldTransform(), x(x), y(y), piece(piece), element(element)
{
  updateTransform();
}

PieceLocation::PieceLocation(const PieceLocation& location)
  : FieldTransform(location), piece(location.piece)
{
  this->x = location.x;
  this->y = location.y;
  this->element = location.element;
}

PieceLocation::~PieceLocation()
{
}

sCoord PieceLocation::GetX() const
{
  return x;
}

sCoord PieceLocation::GetY() const
{
  return y;
}

const Piece& PieceLocation::GetPiece() const
{
  return piece;
}

const FieldElement& PieceLocation::GetElement() const
{
  return element;
}

void PieceLocation::SetX(sCoord x)
{
  this->x = x;
  updateTransform();
}

void PieceLocation::SetY(sCoord y)
{
  this->y = y;
  updateTransform();
}

PieceLocation::operator string() const
{
  return lexical_cast<std::string>(x) + lexical_cast<std::string>(y) + string(piece);
}

bool PieceLocation::operator == (const PieceLocation& rhs) const
{
  return x == rhs.x && y == rhs.y && piece == rhs.piece;
}

PieceLocation& PieceLocation::operator = (const PieceLocation& rhs)
{
  if (&rhs == this)
    return *this;

  this->piece = rhs.piece;
  this->x = rhs.x;
  this->y = rhs.y;
  this->element = rhs.element;
  updateTransform();

  return *this;
}

void PieceLocation::updateTransform()
{
  Reset();
  for (sCoord i = 0; i < numeric_cast<sCoord>(piece.GetWidth() * piece.GetHeight()); ++i)
  {
    uCoord x2 = numeric_cast<uCoord>(i % piece.GetWidth()),
           y2 = numeric_cast<uCoord>(i / piece.GetWidth());
    if (piece(x2, y2))
      (*this)(x + x2, y + y2) = element;
  }
}
