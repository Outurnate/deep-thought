#include "libtetrinet/FieldEvaluator.hpp"

#include <functional>
#include <algorithm>
#include <boost/cast.hpp>
#include <iostream>

#include "libtetrinet/FieldTransform.hpp"
#include "libtetrinet/Field.hpp"
#include "libtetrinet/FieldElement.hpp"
#include "libtetrinet/PieceLocation.hpp"
#include "libtetrinet/PieceShape.hpp"
#include "libtetrinet/PieceRotation.hpp"
#include "libtetrinet/RotationDirection.hpp"

using namespace std;
using namespace boost;

FieldTransform FieldEvaluator::GenerateSheetTransform(const Field& field)
{//TODO revist
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

FieldEvaluator::PieceLocationTransformSet FieldEvaluator::DiscoverTransforms(const Field& field, PieceShape pieceShape)
{
  PieceLocationTransformSet transforms;
  
  for (PieceRotation rotation : { PieceRotation::Z, PieceRotation::R, PieceRotation::T, PieceRotation::L })
  {
    Piece piece = Piece(pieceShape, rotation);

    for (uCoord xf = 0; xf < fieldWidth; ++xf)
      for (uCoord xp = 0; xp < piece.GetWidth(); ++xp)
      {
	if (piece.GetHeightAt(xp))
	{
	  try
	  {
	    transforms.emplace(piece, xf - xp, fieldHeight - field.GetHeightAt(xf) - piece.GetHeightAt(xp).value() - 1);
	    cout << "Accepted x=" << xf - xp << ",y=" << fieldHeight - field.GetHeightAt(xf) - piece.GetHeightAt(xp).value() - 1 << ",fh=" << field.GetHeightAt(xf) << endl;
	  }
	  catch (out_of_range)
	  {
	    cout << "Rejected x=" << xf - xp << ",y=" << fieldHeight - field.GetHeightAt(xf) - piece.GetHeightAt(xp).value() - 1 << ",fh=" << field.GetHeightAt(xf) << endl;
	  }
	}
      }
  }

  for (PieceLocationTransformSet::iterator it = transforms.begin(); it != transforms.end(); )
  {
    if(!it->CanApplyToField(field))
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

bool TryNewLocation(vector<PieceLocation>& locations, const PieceLocation& location, sCoord dx, sCoord dy)
{
  try
  {
    if (((location.GetX() + dx) < static_cast<sCoord>(fieldWidth)) &&
	((location.GetX() + dx) >= 0) &&
	((location.GetY() + dy) < static_cast<sCoord>(fieldHeight)) &&
	((location.GetY() + dy) >= 0))
      locations.push_back(PieceLocation(location.GetPiece(), location.GetX() + dx, location.GetY() + dy));
  }
  catch (out_of_range)
  {
    return false; //TODO
  }
  return true;
}

bool FieldEvaluator::CanEscape(const Field& field, const FieldTransform& escapeRegion, const PieceLocation start, FieldTransform& paint)
{
  paint += start;
  if (escapeRegion && start)
  {
    vector<PieceLocation> locations;
    TryNewLocation(locations, start, 1,  0);
    TryNewLocation(locations, start, -1, 0);
    TryNewLocation(locations, start, 0,  1);
    PieceLocation pCW = PieceLocation(start);
    if (Rotate(pCW, field, RotationDirection::CW))
      locations.push_back(pCW);
    PieceLocation pCCW = PieceLocation(start);
    if (Rotate(pCW, field, RotationDirection::CCW))
      locations.push_back(pCCW);
    for (PieceLocation location : locations)
    {
      //FieldTransform tmpTrans(location, FieldElement::RED);
      if (location.CanApplyToField(field))
      {
	if (!(escapeRegion && location)) // it's a new transform
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

void FieldEvaluator::ValidateTransforms(const Field& field, PieceLocationTransformSet& locations)
{
  FieldTransform sheetTransform(GenerateSheetTransform(field));
  for (unordered_set<PieceLocation>::iterator it = locations.begin(); it != locations.end(); )
  {
    if(*it && sheetTransform && !CanEscape(field, sheetTransform, *it))
      locations.erase(it++);
    else
      ++it;
  }
}

static SRSKickMap srsmap_jlstz = SRSKickMap
{
  {
    PieceRotationPair(PieceRotation::Z, PieceRotation::R),
    {
      TransformPair(  0,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( -1,  +1 ),
      TransformPair(  0,  -2 ),
      TransformPair( -1,  -2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::R, PieceRotation::Z),
    {
      TransformPair(  0,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( +1,  -1 ),
      TransformPair(  0,  +2 ),
      TransformPair( +1,  +2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::R, PieceRotation::T),
    {
      TransformPair(  0,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( +1,  -1 ),
      TransformPair(  0,  +2 ),
      TransformPair( +1,  +2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::T, PieceRotation::R),
    {
      TransformPair(  0,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( -1,  +1 ),
      TransformPair(  0,  -2 ),
      TransformPair( -1,  -2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::T, PieceRotation::L),
    {
      TransformPair(  0,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( +1,  +1 ),
      TransformPair(  0,  -2 ),
      TransformPair( +1,  -2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::L, PieceRotation::T),
    {
      TransformPair(  0,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( -1,  -1 ),
      TransformPair(  0,  +2 ),
      TransformPair( -1,  +2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::L, PieceRotation::Z),
    {
      TransformPair(  0,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( -1,  -1 ),
      TransformPair(  0,  +2 ),
      TransformPair( -1,  +2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::Z, PieceRotation::L),
    {
      TransformPair(  0,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( +1,  +1 ),
      TransformPair(  0,  -2 ),
      TransformPair( +1,  -2 )
    }
  },
}, srsmap_i = SRSKickMap
{
  {
    PieceRotationPair(PieceRotation::Z, PieceRotation::R),
    {
      TransformPair(  0,  0  ),
      TransformPair( -2,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( -2,  -1 ),
      TransformPair( +1,  +2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::R, PieceRotation::Z),
    {
      TransformPair(  0,  0  ),
      TransformPair( +2,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( +2,  +1 ),
      TransformPair( -1,  -2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::R, PieceRotation::T),
    {
      TransformPair(  0,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( +2,  0  ),
      TransformPair( -1,  +2 ),
      TransformPair( +2,  -1 )
    }
  },
  {
    PieceRotationPair(PieceRotation::T, PieceRotation::R),
    {
      TransformPair(  0,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( -2,  0  ),
      TransformPair( +1,  -2 ),
      TransformPair( -2,  +1 )
    }
  },
  {
    PieceRotationPair(PieceRotation::T, PieceRotation::L),
    {
      TransformPair(  0,  0  ),
      TransformPair( +2,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( +2,  +1 ),
      TransformPair( -1,  -2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::L, PieceRotation::T),
    {
      TransformPair(  0,  0  ),
      TransformPair( -2,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( -2,  -1 ),
      TransformPair( +1,  +2 )
    }
  },
  {
    PieceRotationPair(PieceRotation::L, PieceRotation::Z),
    {
      TransformPair(  0,  0  ),
      TransformPair( +1,  0  ),
      TransformPair( -2,  0  ),
      TransformPair( +1,  -2 ),
      TransformPair( -2,  +1 )
    }
  },
  {
    PieceRotationPair(PieceRotation::Z, PieceRotation::L),
    {
      TransformPair(  0,  0  ),
      TransformPair( -1,  0  ),
      TransformPair( +2,  0  ),
      TransformPair( -1,  +2 ),
      TransformPair( +2,  -1 )
    }
  },
};

bool FieldEvaluator::Rotate(PieceLocation& location, const Field& field, RotationDirection direction)
{
  if (location.GetPiece().GetShape() == PieceShape::O)
    return true;
  
  PieceRotation rotation = PieceRotation(uint8_t(location.GetPiece().GetRotation()) + (bool(direction) ? -1 : 1));
  Piece newPiece = Piece(location.GetPiece().GetShape(), rotation);
  for (TransformPair& transform : (location.GetPiece().GetShape() == PieceShape::I ? srsmap_i : srsmap_jlstz)[PieceRotationPair(location.GetPiece().GetRotation(), rotation)])
  {
    try
    {
      FieldTransform ftransform(PieceLocation(newPiece, location.GetX() + transform.first, location.GetY() + transform.second));
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
  vector<unsigned> heights;
  for (unsigned x = 0; x < fieldWidth; ++x)
    heights.push_back(field.GetHeightAt(x));
  return fieldHeight - *std::min_element(heights.begin(), heights.end()); // find the tallest column
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
