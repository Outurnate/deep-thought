#ifndef FIELDTRANSFORM_HPP
#define FIELDTRANSFORM_HPP

#include <memory>

#include "Enum.hpp"

class FieldTransform
{
public:
  FieldTransform(unsigned width = 12, unsigned height = 22);
  
  FieldElement& operator() (uint8_t x, uint8_t y);
  const FieldType::const_iterator begin() const;
  const FieldType::const_iterator end() const;
private:
  const unsigned fieldWidth, fieldHeight, fieldSize;
  
  std::unique_ptr<FieldType> tempfield;
};

#endif
