#ifndef FIELD_HPP
#define FIELD_HPP

#include <boost/core/noncopyable.hpp>
#include <boost/range/any_range.hpp>
#include <memory>

class Field;

#include "libtetrinet/Enum.hpp"
#include "libtetrinet/FieldTransform.hpp"

typedef boost::any_range<
  FieldElement,
  boost::forward_traversal_tag,
  FieldElement,
  std::ptrdiff_t> FieldElementRange;

class Field : private boost::noncopyable
{
  friend std::ostream& operator<< (std::ostream& stream, const Field& field);
public:
  Field(uAxis width = 12, uAxis height = 22);
  ~Field();

  void ApplyTransform(const FieldTransform& transform);
  
  const FieldElement& operator()(uCoord x, uCoord y) const;
  const FieldElement& operator()(uCoord i) const;
  uAxis GetWidth() const;
  uAxis GetHeight() const;
  uAxis GetSize() const;

  const FieldType::const_iterator begin() const;
  const FieldType::const_iterator end() const;

  const FieldElementRange column(uCoord x) const;
  const FieldElementRange row(uCoord y) const;
private:
  const uAxis fieldWidth, fieldHeight, fieldSize;
  std::unique_ptr<FieldType> field;
};

#endif
