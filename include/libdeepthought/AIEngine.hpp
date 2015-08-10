/*
 * AIEngine.h
 *
 *  Created on: Oct 26, 2012
 *      Author: joseph
 */

#ifndef AIENGINE_H
#define AIENGINE_H

#include <boost/asio.hpp>
#include <boost/thread.hpp>

class AIEngine;
enum class AIState;

#include "libdeepthought/AIState.hpp"
#include "libdeepthought/Genome.hpp"
#include "libtetrinet/TetrinetClient.hpp"

/**
 * Core AI class.  Connects to server and responds to messages
 */
class AIEngine : private boost::noncopyable, public TetrinetClient
{
public:
  /**
   * Init the engine with the genome and logger
   */
  AIEngine(Genome genome, log4cxx::LoggerPtr logger);

  // Inherited from TetrinetClient
  void Stop() override;

  /**
   * Gets the current state
   */
  AIState GetState() const;
  /**
   * Sets/clears wait flag
   */
  void SetWait(bool waiting);
private:
  /**
   * Main game loop
   *
   * Calculates pieces and ranks their position, placing them in the highest ranked spot
   */
  void DoPlacing();
  
  // Inherited from TetrinetClient
  virtual void ProcessCommand(TetrinetMessage message, std::deque<std::string>& tokens) override;

  /** Sets current state and fires handler */
  inline void setState(AIState state);

  std::vector<int> freqarr, specarr;
  boost::thread* placer;
  /** Mutex for member field */
  boost::mutex fmtx;
  boost::posix_time::seconds pieceDelay;
  log4cxx::LoggerPtr logger;
  AIState currentState;
};
#endif
