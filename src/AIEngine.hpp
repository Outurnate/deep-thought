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
  int width, height; // pre-calculate these, for speed
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
  void Run();
private:
  virtual void DoPlacing();
  virtual void ProcessCommand(std::string cmd);

  inline int gapCount(const std::vector<char>& _field);
  inline int blockadeCount(const std::vector<char>& _field);
  inline int rowCount(const std::vector<char>& _field);
  inline int clearCount(std::vector<char>& _field);
  inline double rank(int piece, PieceLocation location);
  inline std::string makeHex(int dec);
  inline std::string encode(std::string name, int ip[]);

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
