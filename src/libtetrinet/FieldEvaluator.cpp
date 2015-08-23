#include "libtetrinet/FieldEvaluator.hpp"

#include <functional>
#include <algorithm>
#include <boost/cast.hpp>
#include <iostream>

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

unordered_set<PieceLocation> FieldEvaluator::DiscoverTransforms(const Field& field, PieceShape pieceShape)
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
    for (uCoord x = 0; x < piece.GetWidth(); ++x)
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

  for (unordered_set<PieceLocation>::iterator it = transforms.begin(); it != transforms.end(); )
  {
    if(!it->GetTransform().CanApplyToField(field))
      transforms.erase(it++);
    else
      ++it;
  }
  
  return transforms;
}

bool FieldEvaluator::CanEscape(const Field& field, const FieldTransform& escapeRegion, const PieceLocation start)
{
  FieldTransform paint;
  return CanEscape(field, escapeRegion, start, paint);
}

void TryNewLocation(vector<PieceLocation>& locations, const PieceLocation& location, sCoord dx, sCoord dy)
{
  if (((location.GetX() + dx) < static_cast<sCoord>(fieldWidth)) &&
      ((location.GetX() + dx) >= 0) &&
      ((location.GetY() + dy) < static_cast<sCoord>(fieldHeight)) &&
      ((location.GetY() + dy) >= 0))
    locations.push_back(PieceLocation(location.GetPiece(), location.GetX() + dx, location.GetY() + dy));
}

bool FieldEvaluator::CanEscape(const Field& field, const FieldTransform& escapeRegion, const PieceLocation start, FieldTransform& paint)
{
  paint += start.GetTransform();
  if (escapeRegion && start.GetTransform())
  {
    vector<PieceLocation> locations;
    TryNewLocation(locations, start, 1,  0);
    TryNewLocation(locations, start, -1, 0);
    TryNewLocation(locations, start, 0, -1);
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
	  if (CanEscape(field, escapeRegion, location, paint))
	    return true;
	}
      }
    }
  }
  else
    return true;
  return false;
}

void FieldEvaluator::ValidateTransforms(const Field& field, unordered_set<PieceLocation>& locations)
{
  FieldTransform sheetTransform(GenerateSheetTransform(field));
  for (unordered_set<PieceLocation>::iterator it = locations.begin(); it != locations.end(); )
  {
    if(it->GetTransform() && sheetTransform && !CanEscape(field, sheetTransform, *it))
      locations.erase(it++);
    else
      ++it;
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

unsigned FieldEvaluator::ColumnHeight(const Field& field, unsigned x)
{
  unsigned colHeight = fieldHeight - 1;
  for (unsigned int y = 0; y < fieldHeight; ++y)  // Search this column top-down
    if (field(x, y) != FieldElement::NONE) // block!
    {
      colHeight = y; // this is the top
      break;
    }
  return fieldHeight - colHeight;
}

unsigned FieldEvaluator::GapCount(const Field& field)
{
  unsigned gapCount = 0;
  for (uCoord x = 0; x < fieldWidth; ++x)
  {
    bool foundBlock = false;
    for (uCoord y = 0; y < fieldHeight; ++y) // Search this column top-down
    {
      if (field(x, y) != FieldElement::NONE) // if there's a block
        foundBlock = true; // flag it
      else if (foundBlock) // if there's no block and we've flagged
        ++gapCount;
    }
  }
  return gapCount;
}

unsigned FieldEvaluator::BlockadeCount(const Field& field)
{
  unsigned totalBlock = 0;
  for (uCoord x = 0; x < fieldWidth; ++x)
  {
    unsigned blockCount = 0;
    bool foundBlock = false;
    for (uCoord y = 0; y < fieldHeight; ++y) // Search this column top-down
    {
      if (field(x, y) != FieldElement::NONE) // if there's a block
      {
        foundBlock = true; // flag it
        blockCount++;
      }
      else if (foundBlock) // if we have blocks over us and this is a gap, stop counting
        goto legitimateUseOfGoto;
    }
    blockCount = 0; // reached the bottom of col. without tripping goto, full col., don't count
legitimateUseOfGoto:
    totalBlock += blockCount;
  }
  return totalBlock;
}

unsigned FieldEvaluator::RowCount(const Field& field)
{
  vector<unsigned>* heights = new vector<unsigned>();
  for (unsigned x = 0; x < fieldWidth; ++x)
    heights->push_back(ColumnHeight(field, x));
  return fieldHeight - *std::min_element(heights->begin(), heights->end()); // find the tallest column
}

unsigned FieldEvaluator::ClearCount(const Field& field, FieldTransform& clearTrans)
{
  int clears = 0, crow = 0; // https://www.youtube.com/watch?v=jYmn3Gwn3oI
  for (uCoord i = 0; i < fieldSize; ++i)
  {
    if (field(i) != FieldElement::NONE) ++crow;
    if ((i % fieldWidth) == fieldWidth - 1)
    {
      if (crow == fieldWidth)
      {
        for (uCoord s = (i / fieldWidth) * fieldWidth; s <= i; ++s)
          clearTrans(s) = FieldElement::NONE;
        for (uCoord s = i; s > 0; --s)
          clearTrans(s) = s > fieldWidth ? field(s - fieldWidth) : FieldElement::NONE;
        clears++;
      }
      crow = 0;
    }
  }
  return clears;
}
