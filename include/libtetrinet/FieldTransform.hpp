#ifndef FIELDTRANSFORM_HPP
#define FIELDTRANSFORM_HPP

#include <memory>
#include <map>

class FieldTransform;

#include "libtetrinet/Enum.hpp"
#include "libtetrinet/Field.hpp"
#include "libtetrinet/Piece.hpp"

class FieldTransform
{
  typedef std::map<uCoord, FieldElement> TransformType;
  
  friend std::ostream& operator << (std::ostream& stream, const FieldTransform& fieldTransform);
  friend FieldTransform& operator += (FieldTransform& destination, const FieldTransform& source);
public:
  FieldTransform(const Field& field);
  FieldTransform(const Field& field, const Piece& piece, sCoord x, sCoord y, FieldElement element);
  FieldTransform(const FieldTransform& transform);
  
  FieldElement& operator() (uCoord x, uCoord y);
  FieldElement& operator() (uCoord i);
  
  const FieldElement& operator() (uCoord x, uCoord y) const;
  const FieldElement& operator() (uCoord i) const;
  
  const TransformType::const_iterator begin() const;
  const TransformType::const_iterator end() const;

  bool ContainsTransform(const FieldTransform& transform) const;
private:
  const Field& field;
  const uAxis fieldWidth, fieldHeight, fieldSize;
  
  std::unique_ptr<TransformType> transforms;
};

#endif
