#include "libtetrinet/FieldEvaluator.hpp"

#include <functional>
#include <algorithm>
#include <boost/cast.hpp>
#include <unordered_set>
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

void FieldEvaluator::ValidateTransforms(const Field& field, vector<PieceLocation>& locations)
{
  FieldTransform sheetTransform(GenerateSheetTransform(field));
  locations.erase(remove_if(locations.begin(), locations.end(), [sheetTransform, &field](const PieceLocation& location)
			    {
			      return location.GetTransform() && sheetTransform && !CanEscape(field, sheetTransform, location);
			    }));
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

/* TODO: port

inline double rank(int piece, PieceLocation location)
{
  vector<char> newfield = vector<char>(field);
  place(newfield, piece, location, 1);
  return
      ((double)(clearCount   (&newfield)                        ) * _c)
    + ((double)(blockadeCount(&newfield) - blockadeCount(&field)) * _b)
    + ((double)(rowCount     (&newfield) - rowCount     (&field)) * _r)
    + ((double)(gapCount     (&newfield) - gapCount     (&field)) * _g);
}

inline unsigned columnHeight(const vector<char>* _field, unsigned x)
{
  unsigned colHeight = FIELD_HEIGHT - 1;
  for (unsigned int y = 0; y < FIELD_HEIGHT; ++y)  // Search this column top-down
    if ((*_field)[FIELD_WIDTH * y + x] != '0') // block!
    {
      colHeight = y; // this is the top
      break;
    }
  return FIELD_HEIGHT - colHeight;
}

inline int gapCount(const vector<char>* _field)
{
  unsigned gapCount = 0;
  for (unsigned x = 0; x < FIELD_WIDTH; ++x)
  {
    bool foundBlock = false;
    for (unsigned y = 0; y < FIELD_HEIGHT; ++y) // Search this column top-down
    {
      if ((*_field)[FIELD_WIDTH * y + x] != '0') // if there's a block
        foundBlock = true; // flag it
      else if (foundBlock) // if there's no block and we've flagged
        ++gapCount;
    }
  }
  return gapCount;
}

inline int blockadeCount(const vector<char>* _field)
{
  unsigned totalBlock = 0;
  for (unsigned x = 0; x < FIELD_WIDTH; ++x)
  {
    unsigned blockCount = 0;
    bool foundBlock = false;
    for (unsigned y = 0; y < FIELD_HEIGHT; ++y) // Search this column top-down
    {
      if ((*_field)[FIELD_WIDTH * y + x] != '0') // if there's a block
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

inline int rowCount(const vector<char>* _field)
{
  vector<unsigned>* heights = new vector<unsigned>();
  for (unsigned x = 0; x < FIELD_WIDTH; ++x)
    heights->push_back(columnHeight(_field, x));
  return FIELD_HEIGHT - *std::min_element(heights->begin(), heights->end()); // find the tallest column
}

inline int clearCount(vector<char>* _field)
{
  int clears = 0, crow = 0; // https://www.youtube.com/watch?v=jYmn3Gwn3oI
  for (long unsigned i = 0; i < _field->size(); ++i)
  {
    if ((*_field)[i] != '0') ++crow;
    if ((i % FIELD_WIDTH) == FIELD_WIDTH - 1)
    {
      if (crow == FIELD_WIDTH)
      {
        for (long unsigned s = (i / FIELD_WIDTH) * FIELD_WIDTH; s <= i; ++s)
          (*_field)[s] = '0';
        for (long unsigned s = i; s > 0; --s)
          (*_field)[s] = s > FIELD_WIDTH ? (*_field)[s - FIELD_WIDTH] : '0';
        clears++;
      }
      crow = 0;
    }
  }
  return clears;
}

*/
