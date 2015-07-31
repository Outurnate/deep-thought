#ifndef FIELD_HPP
#define FIELD_HPP

#include <boost/core/noncopyable.hpp>
#include <boost/range/any_range.hpp>
#include <memory>

class Field;

#include "Enum.hpp"
#include "FieldTransform.hpp"

typedef boost::any_range<
  FieldElement,
  boost::forward_traversal_tag,
  FieldElement,
  std::ptrdiff_t> FieldElementRange;

class Field : private boost::noncopyable
{
  friend std::ostream& operator<< (std::ostream& stream, const Field& field);
public:
  Field(Coord width = 12, Coord height = 22);
  ~Field();

  void ApplyTransform(const FieldTransform& transform);
  
  const FieldElement& operator()(Coord x, Coord y) const;
  const FieldElement& operator()(Coord i) const;
  Coord GetWidth() const;
  Coord GetHeight() const;
  Coord GetSize() const;

  const FieldType::const_iterator begin() const;
  const FieldType::const_iterator end() const;

  const FieldElementRange column(Coord x) const;
  const FieldElementRange row(Coord y) const;
private:
  const Coord fieldWidth, fieldHeight, fieldSize;
  std::unique_ptr<FieldType> field;
};

#endif
