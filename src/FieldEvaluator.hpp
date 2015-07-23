#ifndef FIELDEVALULATOR_HPP
#define FIELDEVALULATOR_HPP

#include <vector>

#include "FieldTransform.hpp"

class FieldEvaluator
{
public:
  static FieldTransform GenerateSheetTransform(const Field& field);
  static FieldTransform FillGaps(const Field& field);
  static std::vector<FieldTransform> DiscoverTransforms(const Field& field);
};

#endif
