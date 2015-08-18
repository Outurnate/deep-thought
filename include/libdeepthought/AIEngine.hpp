/*
 * AIEngine.h
 *
 *  Created on: Oct 26, 2012
 *      Author: joseph
 */

#ifndef AIENGINE_H
#define AIENGINE_H

#include "libdeepthought/Genome.hpp"
#include "libtetrinet/TetrinetClient.hpp"

/**
 * Core AI class.  Connects to server and responds to messages
 */
class AIEngine : public TetrinetClient
{
public:
  /**
   * Init the engine with the genome and logger
   */
  AIEngine(const Genome& genome, log4cxx::LoggerPtr logger);
private:
  PieceLocation NewPiece(const Piece& piece) override;
  
  log4cxx::LoggerPtr logger;
  const Genome& genome;
};

#endif
