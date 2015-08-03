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
  for (uAxis x = 0; x < field.GetWidth(); ++x)
    for (uAxis y = 0; y < field.GetHeight(); ++y)
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
    BOOST_WARN_THROW(FieldTransform(field, piece,    field.GetWidth()  + 1, 0, FieldElement::RED), std::out_of_range);
    BOOST_WARN_THROW(FieldTransform(field, piece, 0, field.GetHeight() + 1,    FieldElement::RED), std::out_of_range);
    BOOST_REQUIRE_THROW(FieldTransform(field, piece, -1, -1, FieldElement::RED), std::out_of_range);
  }
  Piece piece = Piece::Get(PieceShape::I, PieceRotation::R);
  BOOST_CHECK(piece(2, 0));
  BOOST_CHECK(piece(2, 1));
  BOOST_CHECK(piece(2, 2));
  BOOST_CHECK(piece(2, 3));
  BOOST_WARN_THROW(piece(5, 5), std::out_of_range);
  field.ApplyTransform(FieldTransform(field, piece, 0, 0, FieldElement::RED));
  BOOST_REQUIRE_EQUAL(field(2, 0), FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(2, 1), FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(2, 2), FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(2, 3), FieldElement::RED);
}

BOOST_AUTO_TEST_CASE(FieldExtents)
{
  Field field;
  Piece piece = Piece::Get(PieceShape::O, PieceRotation::Z);
  field.ApplyTransform(FieldTransform(field, piece, 0,                                   0,                                     FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, piece, field.GetWidth() - piece.GetWidth(), 0,                                     FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, piece, 0,                                   field.GetHeight() - piece.GetHeight(), FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, piece, field.GetWidth() - piece.GetWidth(), field.GetHeight() - piece.GetHeight(), FieldElement::RED));
  BOOST_REQUIRE_EQUAL(field(0,                    0),                     FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(field.GetWidth() - 1, 0),                     FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(0,                    field.GetHeight() - 1), FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(field.GetWidth() - 1, field.GetHeight() - 1), FieldElement::RED);
}

BOOST_AUTO_TEST_CASE(FieldIterators)
{
  Field field;
  Piece piece = Piece::Get(PieceShape::O, PieceRotation::Z);
  field.ApplyTransform(FieldTransform(field, piece, 0,                                   0,                                     FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, piece, field.GetWidth() - piece.GetWidth(), 0,                                     FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, piece, 0,                                   field.GetHeight() - piece.GetHeight(), FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, piece, field.GetWidth() - piece.GetWidth(), field.GetHeight() - piece.GetHeight(), FieldElement::RED));
  for (uCoord x = 0; x < field.GetWidth(); ++x)
  {
    uCoord h = 0;
    BOOST_FOREACH(FieldElement entry, field.column(x))
    {
      BOOST_REQUIRE_EQUAL(entry, field(x, h));
      ++h;
    }
    BOOST_REQUIRE_EQUAL(h, field.GetHeight());
  }
  for (uCoord y = 0; y < field.GetHeight(); ++y)
  {
    uCoord w = 0;
    BOOST_FOREACH(FieldElement entry, field.row(y))
    {
      BOOST_REQUIRE_EQUAL(entry, field(w, y));
      ++w;
    }
    BOOST_REQUIRE_EQUAL(w, field.GetWidth());
  }
}

// Time for some meat

BOOST_AUTO_TEST_CASE(SheetTransformTrivial)
{
  Field field;
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::I, PieceRotation::Z), 2, 9, FieldElement::RED));
  field.ApplyTransform(FieldEvaluator::GenerateSheetTransform(field));
  BOOST_REQUIRE_EQUAL(field(2, 11), FieldElement::UNDEFINED);
  BOOST_REQUIRE_EQUAL(field(1, 11), FieldElement::NONE);
  BOOST_REQUIRE_EQUAL(field(2, 10), FieldElement::RED);
}

BOOST_AUTO_TEST_CASE(GapExploreTrivial)
{
  Field field;
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation::Z), 2, 10, FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation::Z), 4, 10, FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation::Z), 6, 10, FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation::Z), 2, 8, FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation::Z), 6, 8, FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation::Z), 2, 6, FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation::Z), 4, 6, FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation::Z), 6, 6, FieldElement::RED));
  FieldTransform gaps(field);
  FieldEvaluator::FillGap(field, (field.GetWidth() * 8) + 4, gaps);
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
  Piece piece = Piece::Get(PieceShape::J, PieceRotation::Z);
  sCoord x = 0, y = 0;
  {
    Field field;
    field.ApplyTransform(FieldTransform(field, piece, x, y, FieldElement::RED));
    BOOST_REQUIRE_EQUAL(field(0, 0), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(0, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(1, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(2, 1), FieldElement::RED);
  }
  {
    Field field;
    piece.Rotate(field, RotationDirection::CW, x, y);
    field.ApplyTransform(FieldTransform(field, piece, x, y, FieldElement::RED));
    BOOST_REQUIRE_EQUAL(field(1, 0), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(2, 0), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(1, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(1, 2), FieldElement::RED);
  }
  {
    Field field;
    piece.Rotate(field, RotationDirection::CCW, x, y);
    field.ApplyTransform(FieldTransform(field, piece, x, y, FieldElement::RED));
    BOOST_REQUIRE_EQUAL(field(0, 0), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(0, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(1, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(2, 1), FieldElement::RED);
  }
}

BOOST_AUTO_TEST_CASE(WallKickRotation)
{
  Piece piece = Piece::Get(PieceShape::L, PieceRotation::R);
  sCoord x = -1, y = 0;
  {
    Field field;
    field.ApplyTransform(FieldTransform(field, piece, x, y, FieldElement::RED));
    BOOST_REQUIRE_EQUAL(field(0, 0), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(0, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(0, 2), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(1, 2), FieldElement::RED);
  }
  {
    Field field;
    piece.Rotate(field, RotationDirection::CW, x, y);
    field.ApplyTransform(FieldTransform(field, piece, x, y, FieldElement::RED));
    BOOST_REQUIRE_EQUAL(field(0, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(0, 2), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(1, 1), FieldElement::RED);
    BOOST_REQUIRE_EQUAL(field(2, 1), FieldElement::RED);
  }
}

BOOST_AUTO_TEST_SUITE_END()
