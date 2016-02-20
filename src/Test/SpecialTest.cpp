#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Special
#include <boost/test/unit_test.hpp>

#include "libtetrinet/Tetrinet.hpp"

GameSettings settings(0, 0, 0, 0, 0, 0, 0, "1234567", "1234567", true, false, 42);

BOOST_AUTO_TEST_CASE(SpecialSanity)
{
  Field field;
  auto derp = CreateSpecialTransform(SpecialPiece::NUKE, settings, field, boost::none);
  BOOST_CHECK(!derp.Empty());
}

BOOST_AUTO_TEST_CASE(ClearLine)
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
              "000000000000"
              "000000000000"
              "000000000000"
              "101010101010"
              "101010101010"_fd);
  field.ApplyTransform(CreateSpecialTransform(SpecialPiece::CLEARLINE, settings, field, boost::none));
  BOOST_CHECK_EQUAL(field,
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
                    "000000000000"
                    "000000000000"
                    "000000000000"
                    "000000000000"
                    "000000000000"
                    "101010101010"_fd);
}

BOOST_AUTO_TEST_CASE(Nuke)
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
              "000000000000"
              "000000000000"
              "000000000000"
              "101010101010"
              "101010101010"_fd);
  field.ApplyTransform(CreateSpecialTransform(SpecialPiece::NUKE, settings, field, boost::none));
  BOOST_CHECK_EQUAL(field, Field());
}

BOOST_AUTO_TEST_CASE(RandomClear)
{
  Field field("111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"_fd);
  field.ApplyTransform(CreateSpecialTransform(SpecialPiece::RANDOMCLEAR, settings, field, boost::none));
  BOOST_CHECK_PREDICATE(std::not_equal_to<size_t>(), (fieldSize)(std::count_if(field.begin(), field.end(), [](const FieldElement& e)
                                                                               {
                                                                                 return e != FieldElement::NONE;
                                                                               })));
}

BOOST_AUTO_TEST_CASE(Switch)
{
  Field field("111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"
              "111111111111"_fd);
  
  Field field2("000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"
               "000001000000"_fd);
  field.ApplyTransform(CreateSpecialTransform(SpecialPiece::SWITCH, settings, field, field2));
  BOOST_CHECK_EQUAL(field, field2);
}

BOOST_AUTO_TEST_CASE(ClearSpecials)
{
  Field field("111111111111"
              "111111111111"
              "111111111111"
              "11a111111111"
              "111111111111"
              "11c111111111"
              "111111111111"
              "11n111111111"
              "111111111111"
              "1111r1111111"
              "111111111111"
              "1111s1111111"
              "111111111111"
              "11111b111111"
              "111111111111"
              "1111111g1111"
              "111111111111"
              "11111111q111"
              "111111111111"
              "1111111111o1"
              "111111111111"
              "111111111111"_fd);
  field.ApplyTransform(CreateSpecialTransform(SpecialPiece::CLEARSPECIALS, settings, field, boost::none));
  BOOST_CHECK_EQUAL(fieldSize, std::count_if(field.begin(), field.end(), [](const FieldElement& e)
                                             {
                                               return !isSpecial(e);
                                             }));
}

BOOST_AUTO_TEST_CASE(Gravity)
{
  Field field("010101010101"
              "101010101010"
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
              "000000000000"
              "000000000000"
              "000000000000"
              "000000000000"_fd);
  
  Field field2("000000000000"
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
               "000000000000"
               "000000000000"
               "000000000000"
               "000000000000"
               "111111111111"_fd);
  field.ApplyTransform(CreateSpecialTransform(SpecialPiece::GRAVITY, settings, field, boost::none));
  BOOST_CHECK_EQUAL(field, field2);
}

BOOST_AUTO_TEST_CASE(Quake)
{
  Field field("000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"
              "000001000000"_fd);
  field.ApplyTransform(CreateSpecialTransform(SpecialPiece::QUAKE, settings, field, boost::none));
  BOOST_CHECK(!std::all_of(field.column(5).begin(), field.column(5).end(), [](const auto& e) { return e != FieldElement::NONE; }));
}

BOOST_AUTO_TEST_CASE(Bomb)
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
              "000011100000"
              "00001o100000"
              "000011100000"
              "000000000000"
              "000000000000"
              "000000000000"
              "000000000000"
              "000000000000"
              "000000000000"
              "000000000000"
              "000000000000"
              "000000000000"_fd);
  field.ApplyTransform(CreateSpecialTransform(SpecialPiece::BOMB, settings, field, boost::none));
  BOOST_CHECK_EQUAL(fieldSize, std::count_if(field.begin(), field.end(), [](const FieldElement& e)
                                             {
                                               return !isSpecial(e);
                                             }));
}
