#ifndef FIELDEVALULATOR_HPP
#define FIELDEVALULATOR_HPP

#include <vector>
#include <unordered_set>

#include "libtetrinet/PieceLocation.hpp"

class FieldEvaluator
{
public:
  static FieldTransform GenerateSheetTransform(const Field& field);
  static void FillGap(const Field& field, const uCoord start, FieldTransform& result);
  static std::unordered_set<PieceLocation> DiscoverTransforms(const Field& field, PieceShape pieceShape);
  static bool CanEscape(const Field& field, const FieldTransform& escapeRegion, const PieceLocation start);
  static void ValidateTransforms(const Field& field, std::unordered_set<PieceLocation>& locations);
  static bool Rotate(PieceLocation& location, const Field& field, RotationDirection direction);
  static unsigned GapCount(const Field& field);
  static unsigned BlockadeCount(const Field& field);
  static unsigned RowCount(const Field& field);
  static unsigned ClearCount(const Field& field, FieldTransform& clearTrans);
private:
  static bool CanEscape(const Field& field, const FieldTransform& escapeRegion, const PieceLocation start, FieldTransform& paint);
};

#endif
