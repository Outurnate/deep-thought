#ifndef FIELDEVALULATOR_HPP
#define FIELDEVALULATOR_HPP

#include <vector>

#include "libtetrinet/PieceLocation.hpp"

class FieldEvaluator
{
public:
  static FieldTransform GenerateSheetTransform(const Field& field);
  static void FillGap(const Field& field, const uCoord start, FieldTransform& result);
  static std::vector<PieceLocation> DiscoverTransforms(const Field& field, PieceShape pieceShape);
  static bool CanEscape(const Field& field, FieldTransform& paint, FieldTransform& escapeRegion, const PieceLocation start);
  static void ValidateTransforms(const Field& field, std::vector<FieldTransform>& transforms);
  static bool Rotate(PieceLocation& location, const Field& field, RotationDirection direction);
};

#endif
