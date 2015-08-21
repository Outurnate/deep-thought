#include "libtetrinet/FieldTransform.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/cast.hpp>

using namespace std;
using namespace boost;

FieldTransform::FieldTransform()
  : transforms(new TransformType())
{
}

FieldTransform::FieldTransform(const FieldTransform& transform)
  : transforms(new TransformType(*transform.transforms))
{
}

FieldTransform::FieldTransform(const std::string& message)
{
  for (unsigned i = 0; i < message.size(); ++i)
    if (message[i] != char(FieldElement::NONE))
      (*this)(i) = FieldElement(message[i]);
}

FieldTransform& FieldTransform::operator = (const FieldTransform& rhs)
{
  if(this == &rhs)
    return *this;
  
  this->transforms = unique_ptr<TransformType>(new TransformType(*rhs.transforms));

  return *this;
}

ostream& operator << (ostream& os, const FieldTransform& fieldTransform)
{
  for (pair<const uCoord, FieldElement>& element : *fieldTransform.transforms)
    os << (uCoord)element.first << " = " << element.second << endl;
  return os;
}

FieldTransform& operator += (FieldTransform& destination, const FieldTransform& source)
{
  for (const auto& element : source)
    (*destination.transforms)[element.first] = element.second;
  return destination;
}

bool operator == (const FieldTransform& lhs, const FieldTransform& rhs)
{
  return *lhs.transforms == *rhs.transforms;
}

FieldElement& FieldTransform::operator() (uCoord x, uCoord y)
{ 
  return const_cast<FieldElement&>(
    static_cast<const FieldTransform&>(*this)(x, y));
}


FieldElement& FieldTransform::operator() (uCoord i)
{
  return const_cast<FieldElement&>(
    static_cast<const FieldTransform&>(*this)(i));
}

const FieldElement& FieldTransform::operator() (uCoord x, uCoord y) const
{
  if (!(x < fieldWidth))
    throw out_of_range("x = " + lexical_cast<string>(x));
  if (!(y < fieldHeight))
    throw out_of_range("y = " + lexical_cast<string>(y));
  return (*this)((y * fieldWidth) + x);
}

// TODO these violate const, fix
const FieldElement& FieldTransform::operator() (uCoord i) const
{
  if (!(i < fieldSize))
    throw out_of_range("i = " + lexical_cast<string>(i));
  return transforms->count(i) ? transforms->at(i) : (transforms->emplace(i, FieldElement::NONE)).first->second;
}

const FieldTransform::TransformType::const_iterator FieldTransform::begin() const
{
  return transforms->begin();
}

const FieldTransform::TransformType::const_iterator FieldTransform::end() const
{
  return transforms->end();
}

bool operator && (const FieldTransform& lhs, const FieldTransform& rhs)
{
  if (lhs.transforms->size() == 0 || rhs.transforms->size())
    return false;
  return all_of(lhs.transforms->begin(), lhs.transforms->end(), [rhs](const pair<uCoord, FieldElement>& element)
		{
		  return (*rhs.transforms)[element.first] != FieldElement::NONE;
		});
}

string FieldTransform::GetFullFieldString() const
{
  string fstr(fieldSize, char(FieldElement::NONE));
  for (pair<uCoord, FieldElement> element : *transforms)
    fstr[element.first] = char(element.second);
  return fstr;
}

bool FieldTransform::CanApplyToField(const Field& field) const
{
  return all_of(transforms->begin(), transforms->end(), [&field](const pair<uCoord, FieldElement>& element)
		{
		  return (element.second != FieldElement::NONE || element.second != FieldElement::UNDEFINED)
		    && field(element.first) == FieldElement::NONE;
		});
}
