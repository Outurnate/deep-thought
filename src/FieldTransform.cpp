#include "FieldTransform.hpp"

using namespace std;

FieldTransform::FieldTransform(const Field& field)
  : field(field), fieldWidth(field.GetWidth()), fieldHeight(field.GetHeight()),
    fieldSize(fieldWidth * fieldHeight), transforms(new TransformType())
{
}


FieldTransform::FieldTransform(const Field& field, const Piece& piece, Coord x, Coord y, FieldElement element)
  : FieldTransform(field)
{
  for (Coord i = 0; i < (piece.GetWidth() * piece.GetHeight()); ++i)
  {
    Coord x2 = i % piece.GetWidth(),
          y2 = i / piece.GetWidth();
    if (piece(x2, y2))
      (*this)(x + x2, y + y2) = element;
  }
}

ostream& operator << (ostream& os, const FieldTransform& fieldTransform)
{
  for (pair<const Coord, FieldElement>& element : *fieldTransform.transforms)
    os << (int)element.first << " = " << element.second << endl;
  return os;
}

FieldElement& FieldTransform::operator() (Coord x, Coord y)
{
  if (fieldWidth <= x)
    throw out_of_range("x");
  if (fieldHeight <= y)
    throw out_of_range("y");
  return (*transforms)[(y * fieldWidth) + x];
}

const FieldTransform::TransformType::const_iterator FieldTransform::begin() const
{
  return transforms->begin();
}

const FieldTransform::TransformType::const_iterator FieldTransform::end() const
{
  return transforms->end();
}
