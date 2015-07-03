#include "Field.hpp"

#include <vector>
#include <algorithm>

#include "Piece.hpp"

using namespace std;

Field::Field(unsigned width, unsigned height)
  : fieldWidth(width), fieldHeight(height), fieldSize(width * height),
    field(new FieldType(fieldSize, FieldElement::NONE))
{
}

Field::~Field()
{
}

bool Field::IsValidTransform(const FieldTransform& transform)
{
}

void Field::ApplyTransform(const FieldTransform& transform)
{
  copy_if(transform.begin(), transform.end(), field->begin(), [](const FieldElement& e) { return e != FieldElement::UNDEFINED; });
}

const FieldElement& Field::operator()(unsigned x, unsigned y)
{
  return (*field)[(y * fieldWidth) + x];
}

unsigned Field::GetWidth() const
{
  return fieldWidth;
}

unsigned Field::GetHeight() const
{
  return fieldHeight;
}
