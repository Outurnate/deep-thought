#include "FieldEvaluator.hpp"

#include <functional>

using namespace std;

FieldTransform FieldEvaluator::GenerateSheetTransform(const Field& field)
{
  FieldTransform result(field);
  vector<bool> tripped = vector<bool>(field.GetWidth(), false); // will be set to true each column that has a block
  for (Coord y = 0; y < field.GetHeight(); ++y)
    for (Coord x = 0; x < field.GetWidth(); ++x)
    {
      if (!tripped[x])
      {
	if(field(x, y) != FieldElement::NONE)
	  tripped[x] = true;
      }
      else if (field(x, y) == FieldElement::NONE)
	result(x, y) = FieldElement::UNDEFINED;
    }
  return result;
}

void FieldEvaluator::FillGap(const Field& field, const Coord start, FieldTransform& result)
{
  if (field(start) != FieldElement::NONE)
    throw runtime_error("cannot fill gap, start block exists");
  
  result(start) = FieldElement::UNDEFINED;
  for (const Coord location : { Coord(start - field.GetWidth()), Coord(start + field.GetWidth()), Coord(start + 1), Coord(start - 1) })
    if (location < field.GetSize() && field(location) == FieldElement::NONE && result(location) == FieldElement::NONE)
      FillGap(field, location, result);
}

vector<FieldTransform> FieldEvaluator::DiscoverTransforms(const Field& field)
{
  vector<FieldTransform> transforms;

  return transforms;
}
