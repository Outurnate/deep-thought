#include "libtetrinet/Field.hpp"

#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/range/adaptor/strided.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/lexical_cast.hpp>

#include "libtetrinet/Piece.hpp"

using namespace std;
using namespace boost;
using namespace boost::adaptors;

Field::Field(uAxis width, uAxis height)
  : fieldWidth(width), fieldHeight(height), fieldSize(width * height),
    field(new FieldType(fieldSize, FieldElement::NONE))
{
}

Field::~Field()
{
}

ostream& operator<< (ostream& os, const Field& field)
{
  for (FieldElement& element : *field.field)
    os << element;
  return os;
}

void Field::ApplyTransform(const FieldTransform& transform)
{
  for (const pair<const uCoord, FieldElement>& element : transform)
    (*field)[element.first] = element.second;
}

const FieldElement& Field::operator()(uCoord x, uCoord y) const
{
  if (!(x < fieldWidth))
    throw out_of_range("x = " + lexical_cast<string>(x));
  if (!(y < fieldHeight))
    throw out_of_range("y = " + lexical_cast<string>(y));
  return (*field)[(y * fieldWidth) + x];
}

const FieldElement& Field::operator()(uCoord i) const
{
  if (!(i < fieldSize))
    throw out_of_range("i = " + lexical_cast<string>(i));
  return (*field)[i];
}

uAxis Field::GetWidth() const
{
  return fieldWidth;
}

uAxis Field::GetHeight() const
{
  return fieldHeight;
}

uAxis Field::GetSize() const
{
  return fieldSize;
}

const FieldType::const_iterator Field::begin() const
{
  return field->cbegin();
}

const FieldType::const_iterator Field::end() const
{
  return field->cend();
}

const FieldElementRange Field::column(uCoord x) const
{
  return iterator_range<FieldType::const_iterator>(next(field->begin(), x), field->end()) | strided(fieldWidth);
}

const FieldElementRange Field::row(uCoord y) const
{
  const auto start = next(field->begin(), y * fieldWidth);
  return iterator_range<FieldType::const_iterator>(start, next(start, fieldWidth));
}