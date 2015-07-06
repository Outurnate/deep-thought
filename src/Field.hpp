#ifndef FIELD_HPP
#define FIELD_HPP

#include <boost/core/noncopyable.hpp>
#include <memory>

class Field;

#include "Enum.hpp"
#include "FieldTransform.hpp"

class Field : private boost::noncopyable
{
  friend std::ostream& operator<< (std::ostream& stream, const Field& field);
public:
  Field(Coord width = 12, Coord height = 22);
  ~Field();

  bool IsValidTransform(const FieldTransform& transform);
  void ApplyTransform(const FieldTransform& transform);
  
  const FieldElement& operator()(Coord x, Coord y);
  Coord GetWidth() const;
  Coord GetHeight() const;
private:
  const Coord fieldWidth, fieldHeight, fieldSize;
  std::unique_ptr<FieldType> field;
};

#endif
