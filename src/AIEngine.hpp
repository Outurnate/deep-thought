/*
 * AIEngine.h
 *
 *  Created on: Oct 26, 2012
 *      Author: joseph
 */

#ifndef AIENGINE_H_
#define AIENGINE_H_

#include <map>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/functional/hash.hpp>

struct PieceDef
{
  std::vector<bool> def;
  int width, height; // cache these, for speed
};

struct PieceLocation
{
  unsigned int x;
  int y;
  long unsigned int r;
};

struct AdvancedPieceLocation
{
  PieceLocation basic;
  double rank;
  bool valid;
};

class AIEngine
{
public:
  AIEngine(boost::asio::io_service& service, std::string nickname, double g, double b, double r, double c);
  virtual ~AIEngine();

  /**
   * Connect to the given server
   *
   * Manages connection to the server, parses packets and dispenses them to ProcessCommand
   */
  void Run();
private:
  /**
   * Main game loop
   *
   * Calculates pieces and ranks their position, placing them in the highest ranked spot
   */
  virtual void DoPlacing();
  /**
   * Processes a command issued from the server
   */
  virtual void ProcessCommand(std::string cmd);

  // Ranking functions

  /** Counts the gaps in a field */
  inline int gapCount(const std::vector<char>& _field);
  /** Counts the number of blockades (blocks over gaps) in a field */
  inline int blockadeCount(const std::vector<char>& _field);
  /** Counts the height of the board in a field */
  inline int rowCount(const std::vector<char>& _field);
  /** Counts the number of full rows made */
  inline int clearCount(std::vector<char>& _field);

  /** Ranks a given location */
  inline double rank(int piece, PieceLocation location);

  /** TODO */
  inline std::string makeHex(int dec);
  /** TODO */
  inline std::string encode(std::string name, int ip[]);

  /** Player's name */
  const std::string SCREEN_NAME;

  boost::hash<std::string> string_hash;
  std::map<int, std::string> plyrids;
  std::vector<int> freqarr, specarr;
  std::vector<char> field;
  int playernum;
  double _g, _b, _r, _c;
  bool cancelPlacing;
  boost::asio::ip::tcp::socket socket;
  boost::asio::io_service& socio;
  boost::mutex fmtx;
  boost::posix_time::seconds pieceDelay;
  std::vector<std::vector<PieceDef> > pdefs;
};
#endif /* AIENGINE_H_ */
