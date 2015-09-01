#ifndef FIELD_HPP
#define FIELD_HPP

#include <boost/core/noncopyable.hpp>
#include <boost/range/any_range.hpp>
#include <memory>

#include "libtetrinet/TetrinetForward.hpp"

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
  Field(const Field& field);

  void ApplyTransform(const FieldTransform& transform);
  
  const FieldElement& operator()(uCoord x, uCoord y) const;
  const FieldElement& operator()(uCoord i) const;

  operator std::string() const;

  const FieldType::const_iterator begin() const;
  const FieldType::const_iterator end() const;

  const FieldElementRange column(uCoord x) const;
  const FieldElementRange row(uCoord y) const;

  uCoord GetHeightAt(uCoord x) const;
private:
  typedef std::array<uCoord, fieldWidth> HeightCacheType;
  void updateHeightCache() const;

  std::unique_ptr<FieldType> field;
  mutable bool heightCacheDirty;
  mutable HeightCacheType heightCache;
};

#endif
