#include "Field.hpp"

#include <vector>
#include <algorithm>

#include "Piece.hpp"

using namespace std;

Field::Field(Coord width, Coord height)
  : fieldWidth(width), fieldHeight(height), fieldSize(width * height),
    field(new FieldType(fieldSize, FieldElement::NONE))
{
}

Field::~Field()
{
}

ostream& operator<< (ostream& os, const Field& field)
{
  for (FieldElement& element : *field.field)
    os << element;
  return os;
}

bool Field::IsValidTransform(const FieldTransform& transform)
{
}

void Field::ApplyTransform(const FieldTransform& transform)
{
  for (const pair<const Coord, FieldElement>& element : transform)
    (*field)[element.first] = element.second;
}

const FieldElement& Field::operator()(Coord x, Coord y)
{
  return (*field)[(y * fieldWidth) + x];
}

Coord Field::GetWidth() const
{
  return fieldWidth;
}

Coord Field::GetHeight() const
{
  return fieldHeight;
}
