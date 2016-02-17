#include "libtetrinet/FieldTransform.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/cast.hpp>
#include <iostream>

#include "libtetrinet/FieldElement.hpp"
#include "libtetrinet/Field.hpp"

using namespace std;
using namespace boost;

FieldTransform::FieldTransform()
{
}

FieldTransform::FieldTransform(const std::string& message)
{
  for (unsigned i = 0; i < message.size(); ++i)
    if (message[i] != char(FieldElement::NONE))
      (*this)(i) = FieldElement(message[i]);
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
  for (const pair<const uCoord, FieldElement>& element : this->transforms)
    stream << (uCoord)element.first << " = " << element.second << endl;
  return stream;
}

FieldTransform& FieldTransform::operator+= (const FieldTransform& rhs)
{
  for (const auto& element : rhs.transforms)
    this->transforms[element.first] = element.second;
  return *this;
}

bool FieldTransform::operator== (const FieldTransform& rhs) const
{
  return this->transforms == rhs.transforms;
}

bool FieldTransform::operator&& (const FieldTransform& rhs) const
{
  return !(this->transforms.size() == 0 || rhs.transforms.size() == 0) &&
    all_of(this->transforms.begin(), this->transforms.end(), [&rhs](const pair<uCoord, FieldElement>& element)
	   {
	     return rhs.ExistsAt(element.first) && rhs.transforms.find(element.first)->second == element.second;
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
  return transforms.count(i) ? transforms[i] : (transforms.emplace(i, FieldElement::NONE)).first->second;
}

FieldTransform::operator string() const
{
  string fstr(fieldSize, char(FieldElement::NONE));
  for (pair<uCoord, FieldElement> element : transforms)
    fstr[element.first] = char(element.second);
  return fstr;
}

bool FieldTransform::CanApplyToField(const Field& field) const
{
  return all_of(transforms.begin(), transforms.end(), [&field](const pair<uCoord, FieldElement>& element)
		{
		  return (element.second != FieldElement::NONE || element.second != FieldElement::UNDEFINED)
		    && field(element.first) == FieldElement::NONE;
		});
}

void FieldTransform::Reset()
{
  transforms.clear();
}

const FieldTransform::const_iterator FieldTransform::begin() const
{
  return transforms.begin();
}

const FieldTransform::const_iterator FieldTransform::end() const
{
  return transforms.end();
}

bool FieldTransform::Empty() const
{
  return transforms.size() == 0;
}

bool FieldTransform::ExistsAt(uCoord x, uCoord y) const
{
  return ExistsAt((y * fieldWidth) + x);
}

bool FieldTransform::ExistsAt(uCoord i) const
{
  return transforms.find(i) != transforms.end();
}
