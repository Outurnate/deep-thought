#ifndef FIELD_HPP
#define FIELD_HPP

#include <boost/core/noncopyable.hpp>
#include <memory>

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
  unsigned GetWidth() const;
  unsigned GetHeight() const;
private:
  const unsigned fieldWidth, fieldHeight, fieldSize;
  std::unique_ptr<FieldType> field;
};

#endif
