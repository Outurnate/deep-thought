#include "FieldTransform.hpp"

using namespace std;

FieldTransform::FieldTransform(unsigned width, unsigned height)
  : fieldWidth(width), fieldHeight(height), fieldSize(width * height),
    tempfield(new vector<FieldElement>(fieldSize, FieldElement::UNDEFINED))
{
}
  
FieldElement& FieldTransform::operator() (uint8_t x, uint8_t y)
{
  return (*tempfield)[(y * fieldWidth) + x];
}
