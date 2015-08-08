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

void FieldEvaluator::FillGap(const Field& field, const uCoord start, FieldTransform& result) // TODO maybe return?
{
  if (field(start) != FieldElement::NONE)
    throw runtime_error("cannot fill gap, start block exists");
  
  result(start) = FieldElement::UNDEFINED;
  for (const uCoord location : { uCoord(start - fieldWidth), uCoord(start + fieldWidth), uCoord(start + 1), uCoord(start - 1) })
    if (location < fieldSize && field(location) == FieldElement::NONE && result(location) == FieldElement::NONE)
      FillGap(field, location, result);
}

vector<PieceLocation> FieldEvaluator::DiscoverTransforms(const Field& field, PieceShape pieceShape)
{
  unordered_set<PieceLocation> transforms;
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
	transforms.emplace(piece, perch.first + offset.first, perch.second + offset.second);
  }

  vector<PieceLocation> finalTransforms;
  copy_if(transforms.begin(), transforms.end(), finalTransforms.begin(), [&field](const PieceLocation& piece)
	  {
	    return piece.GetTransform().CanApplyToField(field);
	  });
  
  return finalTransforms;
}

bool FieldEvaluator::CanEscape(const Field& field, FieldTransform& paint, FieldTransform& escapeRegion, const PieceLocation start)
{
  paint += start.GetTransform();
  if (escapeRegion && start.GetTransform())
  {
    vector<PieceLocation> locations
    {
      PieceLocation(start.GetPiece(), start.GetX() + 1, start.GetY()),
      PieceLocation(start.GetPiece(), start.GetX() - 1, start.GetY()),
      PieceLocation(start.GetPiece(), start.GetX(),     start.GetY() - 1)
    };
    PieceLocation pCW = PieceLocation(start);
    if (Rotate(pCW, field, RotationDirection::CW))
      locations.push_back(pCW);
    PieceLocation pCCW = PieceLocation(start);
    if (Rotate(pCW, field, RotationDirection::CCW))
      locations.push_back(pCCW);
    for (PieceLocation location : locations)
    {
      //FieldTransform tmpTrans(location, FieldElement::RED);
      if (location.GetTransform().CanApplyToField(field))
      {
	if (!(escapeRegion && location.GetTransform())) // it's a new transform
	{
	  if (CanEscape(field, paint, escapeRegion, location))
	    return true;
	}
      }
    }
  }
  else
    return true;
  return false;
}

void FieldEvaluator::ValidateTransforms(const Field& field, vector<FieldTransform>& transforms)
{
  FieldTransform sheetTransform(GenerateSheetTransform(field));
  for (const FieldTransform& transform : transforms)
    if (transform && sheetTransform)
    {
    }
}

bool FieldEvaluator::Rotate(PieceLocation& location, const Field& field, RotationDirection direction)
{
  if (location.GetPiece().GetShape() == PieceShape::O)
    return true;
  
  PieceRotation rotation = PieceRotation(uint8_t(location.GetPiece().GetRotation()) + (bool(direction) ? -1 : 1));
  Piece newPiece = Piece::Get(location.GetPiece().GetShape(), rotation);
  for (TransformPair& transform : (location.GetPiece().GetShape() == PieceShape::I ? srsmap_i : srsmap_jlstz)[PieceRotationPair(location.GetPiece().GetRotation(), rotation)])
  {
    try
    {
      FieldTransform ftransform(PieceLocation(newPiece, location.GetX() + transform.first, location.GetY() + transform.second).GetTransform());
      if (all_of(ftransform.begin(), ftransform.end(), [&field](pair<uCoord, FieldElement> element)
		 {
		   return element.second != FieldElement::NONE && field(element.first) == FieldElement::NONE;
		 }))
      {
	location = PieceLocation(newPiece, location.GetX() + transform.first, location.GetY() + transform.second);
	return true;
      }
    }
    catch (std::out_of_range) // TODO REMOVE
    {
      continue; // piece was placed off field
    }
  }
  return false;
}
