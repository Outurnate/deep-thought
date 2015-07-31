#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE EngineTest
#include <boost/test/unit_test.hpp>

#include <boost/foreach.hpp>

#include "Field.hpp"
#include "Piece.hpp"
#include "FieldEvaluator.hpp"

BOOST_AUTO_TEST_SUITE(EngineTest)

BOOST_AUTO_TEST_CASE(FieldMemCheck)
{
  Field field;
  for (unsigned x = 0; x < field.GetWidth(); ++x)
    for (unsigned y = 0; y < field.GetHeight(); ++y)
      BOOST_REQUIRE_EQUAL(field(x, y), FieldElement::NONE);
}

BOOST_AUTO_TEST_CASE(TrivialTransform)
{
  Field field;
  {
    Piece piece = Piece::Get(PieceShape::I, PieceRotation(0));
    BOOST_CHECK(piece(0, 0));
    BOOST_CHECK(piece(1, 0));
    BOOST_CHECK(piece(2, 0));
    BOOST_CHECK(piece(3, 0));
    BOOST_WARN_THROW(piece(1, 1), std::out_of_range);
    BOOST_WARN_THROW(FieldTransform(field, piece,    field.GetWidth()  + 1, 0, FieldElement::RED), std::out_of_range);
    BOOST_WARN_THROW(FieldTransform(field, piece, 0, field.GetHeight() + 1,    FieldElement::RED), std::out_of_range);
  }
  Piece piece = Piece::Get(PieceShape::I, PieceRotation(1));
  BOOST_CHECK(piece(0, 0));
  BOOST_CHECK(piece(0, 1));
  BOOST_CHECK(piece(0, 2));
  BOOST_CHECK(piece(0, 3));
  BOOST_WARN_THROW(piece(1, 1), std::out_of_range);
  field.ApplyTransform(FieldTransform(field, piece, 0, 0, FieldElement::RED));
  BOOST_REQUIRE_EQUAL(field(0, 0), FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(0, 1), FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(0, 2), FieldElement::RED);
  BOOST_REQUIRE_EQUAL(field(0, 3), FieldElement::RED);
}

BOOST_AUTO_TEST_CASE(FieldExtents)
{
  Field field;
  Piece piece = Piece::Get(PieceShape::O, PieceRotation(0));
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
  Piece piece = Piece::Get(PieceShape::O, PieceRotation(0));
  field.ApplyTransform(FieldTransform(field, piece, 0,                                   0,                                     FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, piece, field.GetWidth() - piece.GetWidth(), 0,                                     FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, piece, 0,                                   field.GetHeight() - piece.GetHeight(), FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, piece, field.GetWidth() - piece.GetWidth(), field.GetHeight() - piece.GetHeight(), FieldElement::RED));
  for (Coord x = 0; x < field.GetWidth(); ++x)
  {
    Coord h = 0;
    BOOST_FOREACH(FieldElement entry, field.column(x))
    {
      BOOST_REQUIRE_EQUAL(entry, field(x, h));
      ++h;
    }
    BOOST_REQUIRE_EQUAL(h, field.GetHeight());
  }
  for (Coord y = 0; y < field.GetHeight(); ++y)
  {
    Coord w = 0;
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
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::I, PieceRotation(0)), 2, 10, FieldElement::RED));
  field.ApplyTransform(FieldEvaluator::GenerateSheetTransform(field));
  BOOST_REQUIRE_EQUAL(field(2, 11), FieldElement::UNDEFINED);
  BOOST_REQUIRE_EQUAL(field(1, 11), FieldElement::NONE);
  BOOST_REQUIRE_EQUAL(field(2, 10), FieldElement::RED);
}

BOOST_AUTO_TEST_CASE(GapExploreTrivial)
{
  Field field;
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation(0)), 2, 10, FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation(0)), 4, 10, FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation(0)), 6, 10, FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation(0)), 2, 8, FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation(0)), 6, 8, FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation(0)), 2, 6, FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation(0)), 4, 6, FieldElement::RED));
  field.ApplyTransform(FieldTransform(field, Piece::Get(PieceShape::O, PieceRotation(0)), 6, 6, FieldElement::RED));
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

BOOST_AUTO_TEST_SUITE_END()
