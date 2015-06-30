#ifndef FIELD_HPP
#define FIELD_HPP

#include <boost/core/noncopyable.hpp>

#include "Enum.hpp"
#include "FieldTransform.hpp"

class Field : private boost::noncopyable
{
public:
  Field(unsigned width = 12, unsigned height = 22);
  ~Field();

  bool IsValidTransform(const FieldTransform& transform);
  void ApplyTransform(const FieldTransform& transform);
  
  const FieldElement& operator()(unsigned x, unsigned y);
private:
  const unsigned fieldWidth, fieldHeight, fieldSize;
  FieldType field;
};

#endif
