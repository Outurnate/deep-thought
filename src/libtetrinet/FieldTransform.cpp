#include "libtetrinet/FieldTransform.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/cast.hpp>
#include <iostream>

#include "libtetrinet/FieldElement.hpp"
#include "libtetrinet/Field.hpp"

using namespace std;
using namespace boost;

FieldTransform::FieldTransform()
  : transforms(new TransformType())
{
}

FieldTransform::FieldTransform(const FieldTransform& transform)
  : FieldTransform()
{
  for (const auto pair : transform)
    (*this)(pair.first) = pair.second;
}

FieldTransform::FieldTransform(const std::string& message)
  : FieldTransform()
{
  for (unsigned i = 0; i < message.size(); ++i)
    if (message[i] != char(FieldElement::NONE))
      (*this)(i) = FieldElement(message[i]);
}

FieldTransform::~FieldTransform()
{
}

FieldTransform& FieldTransform::operator= (const FieldTransform& rhs)
{
  if(this == &rhs)
    return *this;

  Reset();
  for (const auto pair : *this)
    (*this)(pair.first) = pair.second;

  return *this;
}

ostream& FieldTransform::operator<< (ostream& stream) const
{
  for (pair<const uCoord, FieldElement>& element : *this->transforms)
    stream << (uCoord)element.first << " = " << element.second << endl;
  return stream;
}

FieldTransform& FieldTransform::operator+= (const FieldTransform& rhs)
{
  for (const auto& element : *this->transforms)
    (*rhs.transforms)[element.first] = element.second;
  return *this;
}

bool FieldTransform::operator== (const FieldTransform& rhs) const
{
  return *this->transforms == *rhs.transforms;
}

bool FieldTransform::operator&& (const FieldTransform& rhs) const
{
  return !(this->transforms->size() == 0 || rhs.transforms->size() == 0) &&
    all_of(this->transforms->begin(), this->transforms->end(), [&rhs](const pair<uCoord, FieldElement>& element)
	   {
	     return (*rhs.transforms)[element.first] != FieldElement::NONE;
	   });
}

FieldElement& FieldTransform::operator() (uCoord x, uCoord y)
{
  if (!(x < fieldWidth))
    throw out_of_range("x = " + lexical_cast<string>(x));
  if (!(y < fieldHeight))
    throw out_of_range("y = " + lexical_cast<string>(y));
  return (*this)((y * fieldWidth) + x);
}


FieldElement& FieldTransform::operator() (uCoord i)
{
  if (!(i < fieldSize))
    throw out_of_range("i = " + lexical_cast<string>(i));
  return transforms->count(i) ? transforms->at(i) : (transforms->emplace(i, FieldElement::NONE)).first->second;
}

FieldTransform::operator string() const
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

void FieldTransform::Reset()
{
  transforms.reset(new TransformType());
}

const FieldTransform::const_iterator FieldTransform::begin() const
{
  return transforms->begin();
}

const FieldTransform::const_iterator FieldTransform::end() const
{
  return transforms->end();
}
