#ifndef FIELD_HPP
#define FIELD_HPP

#include <boost/core/noncopyable.hpp>
#include <boost/range/any_range.hpp>
#include <memory>

class Field;

#include "libtetrinet/Enum.hpp"

const uAxis fieldWidth = 12;
const uAxis fieldHeight = 22;
const uAxis fieldSize = fieldWidth * fieldHeight;

#include "libtetrinet/FieldTransform.hpp"

typedef boost::any_range<
  FieldElement,
  boost::forward_traversal_tag,
  FieldElement,
  std::ptrdiff_t> FieldElementRange;

Field operator "" _fd(const char* definition, size_t size);

class Field
{
  friend std::ostream& operator<< (std::ostream& stream, const Field& field);
  friend Field operator "" _fd(const char* definition, size_t size);
public:
  Field();

  void ApplyTransform(const FieldTransform& transform);
  
  const FieldElement& operator()(uCoord x, uCoord y) const;
  const FieldElement& operator()(uCoord i) const;

  const FieldType::const_iterator begin() const;
  const FieldType::const_iterator end() const;

  const FieldElementRange column(uCoord x) const;
  const FieldElementRange row(uCoord y) const;
private:
  std::unique_ptr<FieldType> field;
};

#endif
