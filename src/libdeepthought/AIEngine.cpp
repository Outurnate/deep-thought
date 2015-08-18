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

PieceLocation AIEngine::NewPiece(const Piece& piece)
{
  auto transforms = FieldEvaluator::DiscoverTransforms(GetField(), piece.GetShape());
  FieldEvaluator::ValidateTransforms(GetField(), transforms);
}
