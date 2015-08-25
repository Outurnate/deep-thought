/*
 * AIEngine.cpp
 *
 *  Created on: Oct 26, 2012
 *      Author: joseph
 */

#include <cctype> // lolwhy

#include "libdeepthought/AIEngine.hpp"
#include "libtetrinet/FieldEvaluator.hpp"

using namespace std;
using namespace log4cxx;
using namespace boost;

AIEngine::AIEngine(const Genome& genome, log4cxx::LoggerPtr logger)
  : TetrinetClient(genome.Name, logger), logger(logger), genome(genome)
{
}

double Rank(Field field, PieceLocation piece, const Genome& genome)
{
  FieldTransform clearTrans;
  field.ApplyTransform(piece);
  auto c = FieldEvaluator::ClearCount(field, clearTrans);
  field.ApplyTransform(clearTrans);
  auto r = FieldEvaluator::RowCount(field);
  auto b = FieldEvaluator::BlockadeCount(field);
  auto g = FieldEvaluator::GapCount(field);

  return
    genome.C * c +
    genome.R * r +
    genome.B * b +
    genome.G * g;
}

PieceLocation AIEngine::NewPiece(const Piece& piece)
{
  auto transforms = FieldEvaluator::DiscoverTransforms(GetField(), piece.GetShape());
  FieldEvaluator::ValidateTransforms(GetField(), transforms);
  vector<pair<PieceLocation, double> > ranked;
  for (const auto& transform : transforms)
    ranked.push_back(pair<PieceLocation, double>(transform, Rank(GetField(), transform, genome)));
  return max_element(ranked.begin(), ranked.end(), [](const auto& a, const auto& b)
		     {
		       return a.second < b.second;
		     })->first;
}
