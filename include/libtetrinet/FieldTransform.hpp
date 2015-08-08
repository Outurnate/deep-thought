#ifndef FIELDTRANSFORM_HPP
#define FIELDTRANSFORM_HPP

#include <memory>
#include <map>
#include <functional>
#include <string>

class FieldTransform;

#include "libtetrinet/Enum.hpp"
#include "libtetrinet/Field.hpp"
#include "libtetrinet/Piece.hpp"

class FieldTransform
{
  typedef std::map<uCoord, FieldElement> TransformType;
  
  friend std::ostream& operator << (std::ostream& stream, const FieldTransform& fieldTransform);
  friend FieldTransform& operator += (FieldTransform& destination, const FieldTransform& source);
  friend bool operator == (const FieldTransform& lhs, const FieldTransform& rhs);
  friend bool operator && (const FieldTransform& lhs, const FieldTransform& rhs);
public:
  FieldTransform();
  //FieldTransform(const PieceLocation& location, FieldElement element);
  FieldTransform(const FieldTransform& transform);
  FieldTransform& operator= (const FieldTransform& rhs);
  
  FieldElement& operator() (uCoord x, uCoord y);
  FieldElement& operator() (uCoord i);
  
  const FieldElement& operator() (uCoord x, uCoord y) const;
  const FieldElement& operator() (uCoord i) const;
  
  const TransformType::const_iterator begin() const;
  const TransformType::const_iterator end() const;

  std::string GetFullFieldString() const;
  bool CanApplyToField(const Field& field) const;
private:
  std::unique_ptr<TransformType> transforms;
};

namespace std
{
  template<> struct hash<FieldTransform>
  {
    size_t operator() (const FieldTransform& transform) const
    {
      return std::hash<std::string>()(transform.GetFullFieldString());
    }
  };
}

#endif
