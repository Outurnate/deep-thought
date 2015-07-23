#ifndef FIELDTRANSFORM_HPP
#define FIELDTRANSFORM_HPP

#include <memory>
#include <map>

class FieldTransform;

#include "Enum.hpp"
#include "Field.hpp"
#include "Piece.hpp"

class FieldTransform
{
  typedef std::map<Coord, FieldElement> TransformType;
  
  friend std::ostream& operator << (std::ostream& stream, const FieldTransform& fieldTransform);
  friend FieldTransform& operator += (FieldTransform& destination, const FieldTransform& source);
public:
  FieldTransform(const Field& field);
  FieldTransform(const Field& field, const Piece& piece, Coord x, Coord y, FieldElement element);
  FieldTransform(const FieldTransform& transform);
  
  FieldElement& operator() (Coord x, Coord y);
  const TransformType::const_iterator begin() const;
  const TransformType::const_iterator end() const;

  bool ContainsTransform(const FieldTransform& transform) const;
private:
  const Field& field;
  const Coord fieldWidth, fieldHeight, fieldSize;
  
  std::unique_ptr<TransformType> transforms;
};

#endif
