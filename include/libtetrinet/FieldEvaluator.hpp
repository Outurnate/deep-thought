#ifndef FIELDEVALULATOR_HPP
#define FIELDEVALULATOR_HPP

#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <log4cxx/logger.h>

#include "libtetrinet/TetrinetForward.hpp"

class FieldEvaluator
{
public:
  typedef std::unordered_set<PieceLocation, std::hash<FieldTransform>, std::equal_to<FieldTransform> > PieceLocationTransformSet;

  FieldEvaluator(log4cxx::LoggerPtr logger);
  
  FieldTransform GenerateSheetTransform(const Field& field) const;
  void FillGap(const Field& field, const uCoord start, FieldTransform& result) const;
  PieceLocationTransformSet DiscoverTransforms(const Field& field, PieceShape pieceShape) const;
  bool CanEscape(const Field& field, const FieldTransform& escapeRegion, const PieceLocation start) const;
  bool Rotate(PieceLocation& location, const Field& field, RotationDirection direction) const;
  unsigned GapCount(const Field& field) const;
  unsigned BlockadeCount(const Field& field) const;
  unsigned RowCount(const Field& field) const;
  unsigned ClearCount(const Field& field, FieldTransform& clearTrans) const;
private:
  log4cxx::LoggerPtr logger;

  bool ValidateTransform(const Field& field, const FieldTransform& sheetTransform, PieceLocation& location) const; // TODO inline
  void TryNewLocation(std::vector<PieceLocation>& locations, const PieceLocation& location, sCoord dx, sCoord dy) const;
  bool CanEscape(const Field& field, const FieldTransform& escapeRegion, const PieceLocation start, FieldTransform& paint) const;
};

//log4cxx::LoggerPtr FieldEvaluator::logger = 

#endif
