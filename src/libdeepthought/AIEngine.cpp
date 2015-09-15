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

double AIEngine::Rank(Field field, PieceLocation piece)
{
  FieldTransform clearTrans;
  auto pr = GetEvaluator().RowCount(field); //TODO opt
  auto pb = GetEvaluator().BlockadeCount(field);
  auto pg = GetEvaluator().GapCount(field);
  field.ApplyTransform(piece);
  auto c = GetEvaluator().ClearCount(field, clearTrans);
  field.ApplyTransform(clearTrans);
  auto r = GetEvaluator().RowCount(field);
  auto b = GetEvaluator().BlockadeCount(field);
  auto g = GetEvaluator().GapCount(field);

  LOG4CXX_TRACE(logger, "Ranked piece: " <<
		"x=" << piece.GetX() <<
		",y=" << piece.GetY() <<
		",p=" << char(piece.GetPiece().GetShape()) <<
		"\t pr=" << pr <<
		",pb=" << pb <<
		",pg=" << pg <<
		",c=" << c <<
		",r=" << r <<
		",b=" << b <<
		",g=" << g);

  return
    genome.C * c +
    genome.R * (pr - r) +
    genome.B * (pb - b) +
    genome.G * (pg - g);
}

PieceLocation AIEngine::NewPiece(const Piece& piece)
{
  auto transforms = GetEvaluator().DiscoverTransforms(GetField(), piece.GetShape());
  if (transforms.size() == 0) throw 42;
  vector<pair<PieceLocation, double> > ranked;
  for (const auto& transform : transforms)
    ranked.push_back(pair<PieceLocation, double>(transform, Rank(GetField(), transform)));
  auto ret = max_element(ranked.begin(), ranked.end(), [](const auto& a, const auto& b)
			 {
			   return a.second < b.second;
			 });
  LOG4CXX_TRACE(logger, "Piece value: " << ret->second << ",C=" << genome.C << ",R=" << genome.R << ",B=" << genome.B << ",G=" << genome.G);
  return ret->first;
}
