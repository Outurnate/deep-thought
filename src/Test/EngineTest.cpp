#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE EngineTest
#include <boost/test/unit_test.hpp>

#include <boost/foreach.hpp>

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
    BOOST_WARN_THROW(PieceLocation(piece, fieldWidth + 1, 0              ).GetTransform(), std::out_of_range);
    BOOST_WARN_THROW(PieceLocation(piece, 0,              fieldHeight + 1).GetTransform(), std::out_of_range);
    BOOST_REQUIRE_THROW(PieceLocation(piece, -1, -1).GetTransform(), std::out_of_range);
  }
  Piece piece = Piece::Get(PieceShape::I, PieceRotation::R);
  BOOST_CHECK(piece(2, 0));
  BOOST_CHECK(piece(2, 1));
  BOOST_CHECK(piece(2, 2));
  BOOST_CHECK(piece(2, 3));
  BOOST_WARN_THROW(piece(5, 5), std::out_of_range);
  field.ApplyTransform(PieceLocation(piece, 0, 0).GetTransform());
  BOOST_REQUIRE_EQUAL(field(2, 0), FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(2, 1), FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(2, 2), FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(2, 3), FieldElement::RED);
}

BOOST_AUTO_TEST_CASE(FieldExtents)
{
  Field field;
  Piece piece = Piece::Get(PieceShape::O, PieceRotation::Z);
  field.ApplyTransform(PieceLocation(piece, 0,                             0                              ).GetTransform());
  field.ApplyTransform(PieceLocation(piece, fieldWidth - piece.GetWidth(), 0                              ).GetTransform());
  field.ApplyTransform(PieceLocation(piece, 0,                             fieldHeight - piece.GetHeight()).GetTransform());
  field.ApplyTransform(PieceLocation(piece, fieldWidth - piece.GetWidth(), fieldHeight - piece.GetHeight()).GetTransform());
  BOOST_REQUIRE_EQUAL(field(0,              0),               FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(fieldWidth - 1, 0),               FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(0,              fieldHeight - 1), FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(fieldWidth - 1, fieldHeight - 1), FieldElement::RED);
}

BOOST_AUTO_TEST_CASE(FieldIterators)
{
  Field field;
  Piece piece = Piece::Get(PieceShape::O, PieceRotation::Z);
  field.ApplyTransform(PieceLocation(piece, 0,                             0                              ).GetTransform());
  field.ApplyTransform(PieceLocation(piece, fieldWidth - piece.GetWidth(), 0                              ).GetTransform());
  field.ApplyTransform(PieceLocation(piece, 0,                             fieldHeight - piece.GetHeight()).GetTransform());
  field.ApplyTransform(PieceLocation(piece, fieldWidth - piece.GetWidth(), fieldHeight - piece.GetHeight()).GetTransform());
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
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::I, PieceRotation::Z), 2, 9).GetTransform());
  field.ApplyTransform(FieldEvaluator::GenerateSheetTransform(field));
  BOOST_REQUIRE_EQUAL(field(2, 11), FieldElement::UNDEFINED);
  BOOST_REQUIRE_EQUAL(field(1, 11), FieldElement::NONE);
  BOOST_REQUIRE_EQUAL(field(2, 10), FieldElement::RED);
}

BOOST_AUTO_TEST_CASE(GapExploreTrivial)
{
  Field field;
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 2, 10).GetTransform());
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 4, 10).GetTransform());
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 6, 10).GetTransform());
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 2, 8).GetTransform());
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 6, 8).GetTransform());
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 2, 6).GetTransform());
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 4, 6).GetTransform());
  field.ApplyTransform(PieceLocation(Piece::Get(PieceShape::O, PieceRotation::Z), 6, 6).GetTransform());
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
    field.ApplyTransform(location.GetTransform());
    BOOST_REQUIRE_EQUAL(field(0, 0), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(0, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(1, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(2, 1), FieldElement::RED);
  }
  {
    Field field;
    FieldEvaluator::Rotate(location, field, RotationDirection::CW);
    field.ApplyTransform(location.GetTransform());
    BOOST_REQUIRE_EQUAL(field(1, 0), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(2, 0), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(1, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(1, 2), FieldElement::RED);
  }
  {
    Field field;
    FieldEvaluator::Rotate(location, field, RotationDirection::CCW);
    field.ApplyTransform(location.GetTransform());
    BOOST_REQUIRE_EQUAL(field(0, 0), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(0, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(1, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(2, 1), FieldElement::RED);
  }
}

BOOST_AUTO_TEST_CASE(WallKickRotation)
{
  PieceLocation location(Piece::Get(PieceShape::L, PieceRotation::R), -1, 0);
  {
    Field field;
    field.ApplyTransform(location.GetTransform());
    BOOST_REQUIRE_EQUAL(field(0, 0), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(0, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(0, 2), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(1, 2), FieldElement::RED);
  }
  {
    Field field;
    FieldEvaluator::Rotate(location, field, RotationDirection::CW);
    field.ApplyTransform(location.GetTransform());
    BOOST_REQUIRE_EQUAL(field(0, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(0, 2), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(1, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(2, 1), FieldElement::RED);
  }
}

BOOST_AUTO_TEST_CASE(BlockEscapeNull)
{
  Field field;
  FieldTransform paint, escape;
  BOOST_REQUIRE(FieldEvaluator::CanEscape(field, paint, escape, PieceLocation(Piece::Get(PieceShape::I, PieceRotation::Z), 5, 5)));
}

BOOST_AUTO_TEST_SUITE_END()
