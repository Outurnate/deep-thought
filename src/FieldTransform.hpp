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
  
  friend std::ostream& operator<< (std::ostream& stream, const FieldTransform& fieldTransform);
public:
  FieldTransform(const Field& field);
  FieldTransform(const Field& field, const Piece& piece, Coord x, Coord y, FieldElement element);
  
  FieldElement& operator() (Coord x, Coord y);
  const TransformType::const_iterator begin() const;
  const TransformType::const_iterator end() const;
private:
  const Field& field;
  const Coord fieldWidth, fieldHeight, fieldSize;
  
  std::unique_ptr<TransformType> transforms;
};

#endif