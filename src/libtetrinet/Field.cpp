#include "libtetrinet/Field.hpp"

#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/range/adaptor/strided.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/lexical_cast.hpp>

#include "libtetrinet/Piece.hpp"
#include "libtetrinet/FieldElement.hpp"
#include "libtetrinet/FieldTransform.hpp"

using namespace std;
using namespace boost;
using namespace boost::adaptors;

Field::Field()
  : field(new FieldType(fieldSize, FieldElement::NONE)), heightCacheDirty(true), heightCache()
{
}

Field::Field(const Field& field)
  : field(new FieldType(fieldSize))
{
  for (unsigned i = 0; i < fieldSize; ++i)
    (*this->field)[i] = (*field.field)[i];
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
  {
    if (element.second == FieldElement::UNDEFINED)
      throw runtime_error("Attempted to apply transform containing undefined operations");
    (*field)[element.first] = element.second;
  }
  heightCacheDirty = true;
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

bool Field::operator == (const Field& field) const
{
  for (uCoord i = 0; i < fieldSize; ++i)
    if ((*this)(i) != field(i))
      return false;
  return true;
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

Field operator "" _fd(const char* definition, size_t size)
{
  assert(size == fieldSize);
  Field field;
  for (unsigned i = 0; i < size; ++i)
    field.field->at(i) = static_cast<FieldElement>(definition[i]);
  return field;
}

uCoord Field::GetHeightAt(uCoord x) const
{
  updateHeightCache();
  return heightCache[x];
}

void Field::updateHeightCache() const
{
//  if (!heightCacheDirty) TODO
//    return;
  
  for (uCoord x = 0; x < fieldWidth; ++x)
  {
    heightCache[x] = 0;
    for (uCoord y = 0; y < fieldHeight; ++y) // go to down
      if ((*this)(x, y) != FieldElement::NONE) // if we hit a block
      {
	heightCache[x] = fieldHeight - y;
	break;
      }
  }
  heightCacheDirty = false;
}

Field::operator string() const
{
  string f;
  for (const auto& element : *field)
    f += static_cast<char>(element); // TODO immute?
  return f;
}

void Field::Reset()
{
  for (auto& e : *field)
    e = FieldElement::NONE;
}
