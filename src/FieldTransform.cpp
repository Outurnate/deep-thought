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

FieldTransform::FieldTransform(const FieldTransform& transform)
  : field(transform.field),
    fieldWidth(transform.fieldWidth),
    fieldHeight(transform.fieldHeight),
    fieldSize(transform.fieldSize),
    transforms(new TransformType(*transform.transforms))
{
}

ostream& operator << (ostream& os, const FieldTransform& fieldTransform)
{
  for (pair<const Coord, FieldElement>& element : *fieldTransform.transforms)
    os << (int)element.first << " = " << element.second << endl;
  return os;
}

FieldTransform& operator += (FieldTransform& destination, const FieldTransform& source)
{
  for (const auto& element : source)
    (*destination.transforms)[element.first] = element.second;
  return destination;
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

bool FieldTransform::ContainsTransform(const FieldTransform& transform) const
{
  return all_of(transform.begin(), transform.end(), [this](pair<Coord, FieldElement> element)
		{
		  return (*transforms)[element.first] != FieldElement::NONE;
		});
}
