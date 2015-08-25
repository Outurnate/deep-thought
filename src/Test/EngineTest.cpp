#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE EngineTest
#include <boost/test/unit_test.hpp>

#include <boost/foreach.hpp>
#include <iostream>

#include "libtetrinet/Field.hpp"
#include "libtetrinet/Piece.hpp"
#include "libtetrinet/FieldEvaluator.hpp"

BOOST_AUTO_TEST_SUITE(EngineTest)

BOOST_AUTO_TEST_CASE(FieldMemCheck)
{
  Field field;
  for (uAxis x = 0; x < fieldWidth; ++x)
    for (uAxis y = 0; y < fieldHeight; ++y)
      BOOST_REQUIRE_EQUAL(field(x, y), FieldElement::NONE);
}

BOOST_AUTO_TEST_CASE(TransformCheck)
{
  Field field;
  {
    Piece piece = Piece::Get(PieceShape::I, PieceRotation::Z);
    BOOST_CHECK(piece(0, 1));
    BOOST_CHECK(piece(1, 1));
    BOOST_CHECK(piece(2, 1));
    BOOST_CHECK(piece(3, 1));
    BOOST_WARN_THROW(piece(5, 5), std::out_of_range);
    BOOST_WARN_THROW(PieceLocation(piece, fieldWidth + 1, 0              ), std::out_of_range);
    BOOST_WARN_THROW(PieceLocation(piece, 0,              fieldHeight + 1), std::out_of_range);
    BOOST_REQUIRE_THROW(PieceLocation(piece, -1, -1), std::out_of_range);
  }
  Piece piece = Piece::Get(PieceShape::I, PieceRotation::R);
  PieceLocation location(piece, 0, 0);
  BOOST_CHECK(piece(2, 0));
  BOOST_CHECK(piece(2, 1));
  BOOST_CHECK(piece(2, 2));
  BOOST_CHECK(piece(2, 3));
  BOOST_WARN_THROW(piece(5, 5), std::out_of_range);
  field.ApplyTransform(location);
  BOOST_REQUIRE_EQUAL(field(2, 0), location.GetElement());
  BOOST_REQUIRE_EQUAL(field(2, 1), location.GetElement());
  BOOST_REQUIRE_EQUAL(field(2, 2), location.GetElement());
  BOOST_REQUIRE_EQUAL(field(2, 3), location.GetElement());
}

BOOST_AUTO_TEST_CASE(FieldExtents)
{
  Field field;
  Piece piece = Piece::Get(PieceShape::O, PieceRotation::Z);
  field.ApplyTransform(PieceLocation(piece, 0,                             0,                               FieldElement::RED));
  field.ApplyTransform(PieceLocation(piece, fieldWidth - piece.GetWidth(), 0,                               FieldElement::RED));
  field.ApplyTransform(PieceLocation(piece, 0,                             fieldHeight - piece.GetHeight(), FieldElement::RED));
  field.ApplyTransform(PieceLocation(piece, fieldWidth - piece.GetWidth(), fieldHeight - piece.GetHeight(), FieldElement::RED));
  BOOST_REQUIRE_EQUAL(field(0,              0),               FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(fieldWidth - 1, 0),               FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(0,              fieldHeight - 1), FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(fieldWidth - 1, fieldHeight - 1), FieldElement::RED);
}

BOOST_AUTO_TEST_CASE(FieldIterators)
{
  Field field;
  Piece piece = Piece::Get(PieceShape::O, PieceRotation::Z);
  field.ApplyTransform(PieceLocation(piece, 0,                             0                              ));
  field.ApplyTransform(PieceLocation(piece, fieldWidth - piece.GetWidth(), 0                              ));
  field.ApplyTransform(PieceLocation(piece, 0,                             fieldHeight - piece.GetHeight()));
  field.ApplyTransform(PieceLocation(piece, fieldWidth - piece.GetWidth(), fieldHeight - piece.GetHeight()));
  for (uCoord x = 0; x < fieldWidth; ++x)
  {
    uCoord h = 0;
    BOOST_FOREACH(FieldElement entry, field.column(x))
    {
      BOOST_REQUIRE_EQUAL(entry, field(x, h));
      ++h;
    }
    BOOST_REQUIRE_EQUAL(h, fieldHeight);
  }
  for (uCoord y = 0; y < fieldHeight; ++y)
  {
    uCoord w = 0;
    BOOST_FOREACH(FieldElement entry, field.row(y))
    {
      BOOST_REQUIRE_EQUAL(entry, field(w, y));
      ++w;
    }
    BOOST_REQUIRE_EQUAL(w, fieldWidth);
  }
}

// Time for some meat

BOOST_AUTO_TEST_CASE(SheetTransformTrivial)
{
  Field field;
  PieceLocation location(Piece::Get(PieceShape::I, PieceRotation::Z), 2, 9);
  field.ApplyTransform(location);
  field.ApplyTransform(FieldEvaluator::GenerateSheetTransform(field));
  BOOST_REQUIRE_EQUAL(field(2, 11), FieldElement::UNDEFINED);
  BOOST_REQUIRE_EQUAL(field(1, 11), FieldElement::NONE);
  BOOST_REQUIRE_EQUAL(field(2, 10), location.GetElement());
}

BOOST_AUTO_TEST_CASE(GapExploreTrivial)
{
  Field field;
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 2, 10));
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 4, 10));
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 6, 10));
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 2, 8));
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 6, 8));
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 2, 6));
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 4, 6));
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 6, 6));
  FieldTransform gaps;
  FieldEvaluator::FillGap(field, (fieldWidth * 8) + 4, gaps);
  field.ApplyTransform(gaps);
  BOOST_REQUIRE_EQUAL(field(4, 8), FieldElement::UNDEFINED);
  BOOST_REQUIRE_EQUAL(field(4, 9), FieldElement::UNDEFINED);
  BOOST_REQUIRE_EQUAL(field(5, 8), FieldElement::UNDEFINED);
  BOOST_REQUIRE_EQUAL(field(5, 9), FieldElement::UNDEFINED);
  BOOST_REQUIRE_EQUAL(field(0, 8), FieldElement::NONE);
  BOOST_REQUIRE_EQUAL(field(0, 9), FieldElement::NONE);
}

BOOST_AUTO_TEST_CASE(SimpleRotation)
{
  PieceLocation location(Piece::Get(PieceShape::J, PieceRotation::Z), 0, 0);
  {
    Field field;
    field.ApplyTransform(location);
    BOOST_REQUIRE_EQUAL(field(0, 0), location.GetElement());
    BOOST_REQUIRE_EQUAL(field(0, 1), location.GetElement());
    BOOST_REQUIRE_EQUAL(field(1, 1), location.GetElement());
    BOOST_REQUIRE_EQUAL(field(2, 1), location.GetElement());
  }
  {
    Field field;
    FieldEvaluator::Rotate(location, field, RotationDirection::CW);
    field.ApplyTransform(location);
    BOOST_REQUIRE_EQUAL(field(1, 0), location.GetElement());
    BOOST_REQUIRE_EQUAL(field(2, 0), location.GetElement());
    BOOST_REQUIRE_EQUAL(field(1, 1), location.GetElement());
    BOOST_REQUIRE_EQUAL(field(1, 2), location.GetElement());
  }
  {
    Field field;
    FieldEvaluator::Rotate(location, field, RotationDirection::CCW);
    field.ApplyTransform(location);
    BOOST_REQUIRE_EQUAL(field(0, 0), location.GetElement());
    BOOST_REQUIRE_EQUAL(field(0, 1), location.GetElement());
    BOOST_REQUIRE_EQUAL(field(1, 1), location.GetElement());
    BOOST_REQUIRE_EQUAL(field(2, 1), location.GetElement());
  }
}

BOOST_AUTO_TEST_CASE(WallKickRotation)
{
  PieceLocation location(Piece::Get(PieceShape::L, PieceRotation::R), -1, 0);
  {
    Field field;
    field.ApplyTransform(location);
    BOOST_REQUIRE_EQUAL(field(0, 0), location.GetElement());
    BOOST_REQUIRE_EQUAL(field(0, 1), location.GetElement());
    BOOST_REQUIRE_EQUAL(field(0, 2), location.GetElement());
    BOOST_REQUIRE_EQUAL(field(1, 2), location.GetElement());
  }
  {
    Field field;
    FieldEvaluator::Rotate(location, field, RotationDirection::CW);
    field.ApplyTransform(location);
    BOOST_REQUIRE_EQUAL(field(0, 1), location.GetElement());
    BOOST_REQUIRE_EQUAL(field(0, 2), location.GetElement());
    BOOST_REQUIRE_EQUAL(field(1, 1), location.GetElement());
    BOOST_REQUIRE_EQUAL(field(2, 1), location.GetElement());
  }
}

BOOST_AUTO_TEST_CASE(NullAmpTransform)
{
  BOOST_REQUIRE(!(FieldTransform() && PieceLocation(Piece::Get(PieceShape::I, PieceRotation::Z), 0, 0)));
}

BOOST_AUTO_TEST_CASE(BlockEscapeNull)
{
  Field field;
  FieldTransform escape;
  BOOST_REQUIRE(FieldEvaluator::CanEscape(field, escape, PieceLocation(Piece::Get(PieceShape::I, PieceRotation::Z), 5, 5)));
}

BOOST_AUTO_TEST_CASE(BlockEscape)
{
  {
    Field field("000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"111100000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"_fd);
    FieldTransform sheet(FieldEvaluator::GenerateSheetTransform(field));
    PieceLocation location(Piece::Get(PieceShape::O, PieceRotation::Z), 0, 19);
    BOOST_REQUIRE(sheet && location);
    BOOST_REQUIRE(!FieldEvaluator::CanEscape(field, sheet, location));
  }
  {
    Field field("000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"000000000000"
		"011110000000"
		"100001000000"
		"100001000000"
		"100001000000"
		"100001000000"_fd);
    FieldTransform sheet(FieldEvaluator::GenerateSheetTransform(field));
    PieceLocation location(Piece::Get(PieceShape::O, PieceRotation::Z), 1, 19);
    BOOST_REQUIRE(sheet && location);
    BOOST_REQUIRE(!FieldEvaluator::CanEscape(field, sheet, location));
  }
}

BOOST_AUTO_TEST_SUITE_END()
