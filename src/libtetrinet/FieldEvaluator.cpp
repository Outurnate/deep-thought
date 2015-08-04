#include "libtetrinet/FieldEvaluator.hpp"

#include <functional>
#include <algorithm>
#include <boost/cast.hpp>
#include <unordered_set>

using namespace std;
using namespace boost;

FieldTransform FieldEvaluator::GenerateSheetTransform(const Field& field)
{
  FieldTransform result;
  vector<bool> tripped = vector<bool>(fieldWidth, false); // will be set to true each column that has a block
  for (uCoord y = 0; y < fieldHeight; ++y)
    for (uCoord x = 0; x < fieldWidth; ++x)
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

void FieldEvaluator::FillGap(const Field& field, const uCoord start, FieldTransform& result)
{
  if (field(start) != FieldElement::NONE)
    throw runtime_error("cannot fill gap, start block exists");
  
  result(start) = FieldElement::UNDEFINED;
  for (const uCoord location : { uCoord(start - fieldWidth), uCoord(start + fieldWidth), uCoord(start + 1), uCoord(start - 1) })
    if (location < fieldSize && field(location) == FieldElement::NONE && result(location) == FieldElement::NONE)
      FillGap(field, location, result);
}

vector<FieldTransform> FieldEvaluator::DiscoverTransforms(const Field& field, PieceShape pieceShape, FieldElement color)
{
  unordered_set<FieldTransform> transforms;
  vector<TransformPair> perches;

  for (uCoord x = 0; x < fieldWidth; ++x)
  {
    bool prev = true;
    for (uCoord y = fieldHeight - 1; y < fieldHeight; --y) // go up
    {
      if (prev && field(x, y) != FieldElement::NONE) // if last block and not current block
	perches.push_back(TransformPair(x, y));
      prev = field(x, y) != FieldElement::NONE;
    }
  }
  
  for (PieceRotation rotation : { PieceRotation::Z, PieceRotation::R, PieceRotation::T, PieceRotation::L })
  {
    Piece piece = Piece::Get(pieceShape, rotation);
    
    vector<TransformPair> columnOffsets; // might want to cache these; could be done compile time for eff.
    for (uCoord x = 0; x < fieldWidth; ++x)
    {
      // last found block
      sCoord ylast = -1; // will never be correct; used as a symbolic value
      for (uCoord y = 0; y < piece.GetHeight(); ++y)
	if (piece(x, y))
	  ylast = y;
      if (ylast != -1)
	columnOffsets.push_back(TransformPair(-numeric_cast<sCoord>(x), -ylast));
    }

    for (TransformPair perch : perches)
      for (TransformPair offset : columnOffsets)
	transforms.emplace(piece, perch.first + offset.first, perch.second + offset.second, color);
  }

  vector<FieldTransform> finalTransforms;
  copy_if(transforms.begin(), transforms.end(), finalTransforms.begin(), [&field](const FieldTransform& transform)
	  {
	    return transform.CanApplyToField(field);
	  });
  
  return finalTransforms;
}
