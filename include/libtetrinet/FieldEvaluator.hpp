#ifndef FIELDEVALULATOR_HPP
#define FIELDEVALULATOR_HPP

#include <vector>

#include "libtetrinet/FieldTransform.hpp"

class FieldEvaluator
{
public:
  static FieldTransform GenerateSheetTransform(const Field& field);
  static void FillGap(const Field& field, const uCoord start, FieldTransform& result);
  static std::vector<FieldTransform> DiscoverTransforms(const Field& field, PieceShape pieceShape, FieldElement color);
};

#endif
