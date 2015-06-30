#include "Field.hpp"

#include <vector>

#include "Piece.hpp"

using namespace std;

Field::Field(unsigned width, unsigned height)
  : fieldWidth(width), fieldHeight(height), fieldSize(width * height),
    field()
{
  Piece *piece;
}

Field::~Field()
{
}

bool Field::IsValidTransform(const FieldTransform& transform)
{
}

void Field::ApplyTransform(const FieldTransform& transform)
{
}

const FieldElement& Field::operator()(unsigned x, unsigned y)
{
}
