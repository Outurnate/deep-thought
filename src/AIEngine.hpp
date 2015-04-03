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

#include "AIStatus.hpp"
#include "AIManager.hpp"
#include "TetrinetClient.hpp"
#include "Genome.hpp"

/** Definition of a tetramino shape */
struct PieceDef
{
  /** Field-based vector of block locations */
  const bool* def;
  /** Width and height occupied by piece and no. of rotation states */
  unsigned width, height, rstates;
};

/** Represents placement instructions for a piece on the field */
struct PieceLocation
{
  /** Coordinates */
  unsigned x, y;
  /** Rotation */
  long unsigned r;
  /** Rank */
  double rank;
};


enum class AIState // should be own header
{
  IDLE,
  RUNNING,
  WAITING
};

/**
 * Core AI class.  Connects to server and responds to messages
 */
class AIEngine : public TetrinetClient
{
public:
  /**
   * Init the engine with the genome and logger
   */
  AIEngine(Genome genome, log4cxx::LoggerPtr logger);
  /**
   * Copy constructor
   */
  AIEngine(const AIEngine& engine);
  /**
   * Destructor
   */
  virtual ~AIEngine();

  // Inherited from TetrinetClient
  void Stop() override;

  /**
   * Gets the current state
   */
  AIState GetState();
  /**
   * Sets/clears wait flag
   */
  AIState SetWait(bool waiting);
  /**
   * Binds this engine to an AIManager
   */
  void BindManager(AIManager* manager);
private:
  /**
   * Main game loop
   *
   * Calculates pieces and ranks their position, placing them in the highest ranked spot
   */
  void DoPlacing();
  
  // Inherited from TetrinetClient
  void ProcessCommand(TetrinetMessage message, std::deque<std::string>* tokens) override;

  // Ranking functions

  /** Counts the gaps in a field */
  inline int gapCount(const std::vector<char>* _field);
  /** Counts the number of blockades (blocks over gaps) in a field */
  inline int blockadeCount(const std::vector<char>* _field);
  /** Counts the height of the board in a field */
  inline int rowCount(const std::vector<char>* _field);
  /** Counts the number of full rows made */
  inline int clearCount(std::vector<char>* _field);

  /** Ranks a given location */
  inline double rank(int piece, PieceLocation location);

  /** Copies piece onto field */
  inline void place(std::vector<char>& _field, unsigned piece, PieceLocation location, unsigned col);
  /** Counts the height of blocks in a given column */
  inline unsigned columnHeight(const std::vector<char>* _field, unsigned x);

  /** Sets current state and fires handler */
  inline void setState(AIState state);

  std::vector<int> freqarr, specarr;
  /** Playing field (row major) */
  std::vector<char> field;
  double _g, _b, _r, _c;
  boost::thread* placer;
  /** Mutex for member field */
  boost::mutex fmtx;
  boost::posix_time::seconds pieceDelay;
  AIManager* manager;
  log4cxx::LoggerPtr logger;
  AIState currentState;

  static constexpr PieceDef *pdefs[7]
  {
    std::move((PieceDef[])
    {
      // I
      PieceDef { std::move((bool[]) { false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true }), 4, 1, 2 },
      PieceDef { std::move((bool[]) { true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false }), 1, 4, 2 }
    }),
    std::move((PieceDef[])
    {
      // O
      PieceDef { std::move((bool[]) { false, false, false, false, false, false, false, false, true, true, false, false, true, true, false, false }), 2, 2, 1 }
    }),
    std::move((PieceDef[])
    {
      // J
      PieceDef { std::move((bool[]) { false, false, false, false, false, false, false, false, true, false, false, false, true, true, true, false }), 3, 2, 4 },
      PieceDef { std::move((bool[]) { false, false, false, false, false, true, false, false, false, true, false, false, true, true, false, false }), 2, 3, 4 },
      PieceDef { std::move((bool[]) { false, false, false, false, false, false, false, false, true, true, true, false, false, false, true, false }), 3, 2, 4 },
      PieceDef { std::move((bool[]) { false, false, false, false, true, true, false, false, true, false, false, false, true, false, false, false }), 2, 3, 4 }
    }),
    std::move((PieceDef[])
    {
      // L
      PieceDef { std::move((bool[]) { false, false, false, false, false, false, false, false, true, true, true, false, true, false, false, false }), 3, 2, 4 },
      PieceDef { std::move((bool[]) { false, false, false, false, true, false, false, false, true, false, false, false, true, true, false, false }), 2, 3, 4 },
      PieceDef { std::move((bool[]) { false, false, false, false, false, false, false, false, false, false, true, false, true, true, true, false }), 3, 2, 4 },
      PieceDef { std::move((bool[]) { false, false, false, false, true, true, false, false, false, true, false, false, false, true, false, false }), 2, 3, 4 }
    }),
    std::move((PieceDef[])
    {
      // S
      PieceDef { std::move((bool[]) { false, false, false, false, false, false, false, false, false, true, true, false, true, true, false, false }), 3, 2, 2 },
      PieceDef { std::move((bool[]) { false, false, false, false, true, false, false, false, true, true, false, false, false, true, false, false }), 2, 3, 2 }
    }),
    std::move((PieceDef[])
    {
      // Z
      PieceDef { std::move((bool[]) { false, false, false, false, false, false, false, false, true, true, false, false, false, true, true, false }), 3, 2, 2 },
      PieceDef { std::move((bool[]) { false, false, false, false, false, true, false, false, true, true, false, false, true, false, false, false }), 2, 3, 2 }
    }),
    std::move((PieceDef[])
    {
      // T
      PieceDef { std::move((bool[]) { false, false, false, false, false, false, false, false, false, true, false, false, true, true, true, false }), 3, 2, 4 },
      PieceDef { std::move((bool[]) { false, false, false, false, false, true, false, false, true, true, false, false, false, true, false, false }), 2, 3, 4 },
      PieceDef { std::move((bool[]) { false, false, false, false, false, false, false, false, true, true, true, false, false, true, false, false }), 3, 2, 4 },
      PieceDef { std::move((bool[]) { false, false, false, false, true, false, false, false, true, true, false, false, true, false, false, false }), 2, 3, 4 }
    })
  }; // pdefs[tetramino][orientation];
};
#endif
