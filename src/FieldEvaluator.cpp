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
      if (!tripped[x] && field(x, y) != FieldElement::NONE)
	tripped[x] = true;
      else if (field(x, y) != FieldElement::NONE)
	result(x, y) = FieldElement::UNDEFINED;
    }
  return result;
}

/**
 * Traverses a field recursively
 * field: source field to traverse
 * initialTransform: starting transform position
 * traversalFunction: traversal function
 *   returns: list of resulting transforms
 *   1: parent field
 *   2: current transform to be evaluated
 * initialFlags: contains all space covered by the algorithm
 */
void FieldTraverse(const Field& field, const FieldTransform& initialTransform, const function<vector<FieldTransform>(const Field&, const FieldTransform&)> traversalFunction, FieldTransform& initialFlags)
{
  vector<FieldTransform> traversals = traversalFunction(field, initialTransform);
  for (FieldTransform traversal : traversals)
  {
    if (!initialFlags.ContainsTransform(traversal))
      FieldTraverse(field, traversal, traversalFunction, initialFlags += traversal);
  }
}

FieldTransform FieldEvaluator::FillGaps(const Field& field)
{
  FieldTransform result(field),
    sheet = GenerateSheetTransform(field),
    map = sheet; // will clear the items from this as we find them
  
  return result;
}

vector<FieldTransform> FieldEvaluator::DiscoverTransforms(const Field& field)
{
  vector<FieldTransform> transforms;

  return transforms;
}
