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
using namespace log4cxx;

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
  FieldTransform sheet(GenerateSheetTransform(field));
  
  for (PieceRotation rotation : { PieceRotation::Z, PieceRotation::R, PieceRotation::T, PieceRotation::L })
  {
    Piece piece = Piece(pieceShape, rotation);

    for (uCoord xf = 0; xf < fieldWidth; ++xf)
      for (uCoord xp = 0; xp < piece.GetWidth(); ++xp)
	if (piece.GetHeightAt(xp))
	{
	  try
	  {
	    PieceLocation tmp(piece, xf - xp, fieldHeight - field.GetHeightAt(xf) - piece.GetHeightAt(xp).value() - 1);
	    if (ValidateTransform(field, sheet, tmp))
	    {
	      if (tmp.CanApplyToField(field))
	      {
		LOG4CXX_TRACE(field.GetLogger(),
			      "Accepted x=" << xf - xp <<
			      ",y=" << fieldHeight - field.GetHeightAt(xf) - piece.GetHeightAt(xp).value() - 1 <<
			      ",fh=" << field.GetHeightAt(xf));
		transforms.insert(tmp);
	      }
	      else
		LOG4CXX_TRACE(field.GetLogger(),
			      "Rejected (off field) x=" << tmp.GetX() <<
			      ",y=" << tmp.GetY() <<
			      ",p=" << char(tmp.GetPiece().GetShape()) <<
			      ",r=" << char('0' + char(tmp.GetPiece().GetRotation())));
	    }
	    else
	      LOG4CXX_TRACE(field.GetLogger(),
			    "Invalidated x=" << tmp.GetX() <<
			    ",y=" << tmp.GetY() <<
			    ",p=" << char(tmp.GetPiece().GetShape()) <<
			    ",r=" << char('0' + char(tmp.GetPiece().GetRotation())));
	  }
	  catch (out_of_range) // TODO remove case
	  {
	    LOG4CXX_TRACE(field.GetLogger(),
			  "Rejected (invalid) x=" << xf - xp <<
			  ",y=" << fieldHeight - field.GetHeightAt(xf) - piece.GetHeightAt(xp).value() - 1 <<
			  ",fh=" << field.GetHeightAt(xf));
	  }
	}
  }
  
  return transforms;
}

bool FieldEvaluator::CanEscape(const Field& field, const FieldTransform& escapeRegion, const PieceLocation start)
{
  FieldTransform paint;
  return CanEscape(field, escapeRegion, start, paint);
}

void FieldEvaluator::TryNewLocation(const Field& field, vector<PieceLocation>& locations, const PieceLocation& location, sCoord dx, sCoord dy)
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
    LOG4CXX_TRACE(field.GetLogger(), "Rejected (translation) x=" << (location.GetX() + dx) << ",y=" << (location.GetY() + dy)); //TODO
    return;
  }
}

bool FieldEvaluator::CanEscape(const Field& field, const FieldTransform& escapeRegion, const PieceLocation start, FieldTransform& paint)
{
  paint += start;
  if (escapeRegion && start)
  {
    vector<PieceLocation> locations;
    TryNewLocation(field, locations, start, 1,  0);
    TryNewLocation(field, locations, start, -1, 0);
    TryNewLocation(field, locations, start, 0,  1);
    if (start.GetPiece().GetShape() != PieceShape::O)
    {
      PieceLocation pCW = PieceLocation(start);
      if (Rotate(pCW, field, RotationDirection::CW))
	locations.push_back(pCW);
      PieceLocation pCCW = PieceLocation(start);
      if (Rotate(pCCW, field, RotationDirection::CCW))
	locations.push_back(pCCW);
    }
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

bool FieldEvaluator::ValidateTransform(const Field& field, const FieldTransform& sheetTransform, PieceLocation& location)
{
  return true;//!((location && sheetTransform) && !CanEscape(field, sheetTransform, location));
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
  return *std::max_element(heights.begin(), heights.end()); // find the tallest column
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
