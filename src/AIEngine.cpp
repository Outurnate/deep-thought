/*
 * AIEngine.cpp
 *
 *  Created on: Oct 26, 2012
 *      Author: joseph
 */

#include <cctype>

#include "AIEngine.hpp"

#include "Constants.hpp"

constexpr PieceDef *AIEngine::pdefs[7];

using boost::asio::ip::tcp;

using namespace std;
using namespace log4cxx;
using namespace boost;

AIEngine::AIEngine(string nickname, double g, double b, double r, double c, AIManager* manager, LoggerPtr logger)
  : TetrinetClient(nickname, logger), field(FIELD_SIZE, '0'), _g(g), _b(b), _r(r), _c(c), pieceDelay(1), manager(manager), logger(logger), placer(0)
{
}

AIEngine::AIEngine(const AIEngine &engine)
  : TetrinetClient(engine), pieceDelay(engine.pieceDelay.total_seconds())
{
  freqarr = engine.freqarr;
  specarr = engine.specarr;
  field = engine.field;
  _g = engine._g;
  _b = engine._b;
  _r = engine._r;
  _c = engine._c;
  logger = engine.logger;
}

AIEngine::~AIEngine()
{
  //TODO: close socket and io_service
}

void AIEngine::Stop()
{
  if (placer)
  {
    placer->interrupt();
    placer->join();
  }
}

void AIEngine::DoPlacing()
{
  this_thread::sleep(pieceDelay);
  while (2 > 1)
  {
    unsigned piece = freqarr[rand() % 100] - 1; // grab from manager...later (TODO)

    fmtx.lock();
    vector<PieceLocation> placements;

    vector<char> ffield(field); // field with all gaps filled
    for (unsigned x = 0; x < FIELD_WIDTH; ++x)
      for (unsigned y = 1; y < FIELD_HEIGHT; ++y)
        if (ffield[FIELD_WIDTH * y + x] == '0' && // bounds check only on right because unsigned
            !((x - 1) < FIELD_WIDTH  && ffield[(FIELD_WIDTH *  y) + (x - 1)] == '0') &&
            !((x + 1) < FIELD_WIDTH  && ffield[(FIELD_WIDTH *  y) + (x - 1)] == '0') &&
            !((y - 1) < FIELD_HEIGHT && ffield[(FIELD_WIDTH * (y - 1)) + x]  == '0') &&
            !((y + 1) < FIELD_HEIGHT && ffield[(FIELD_WIDTH * (y + 1)) + x]  == '0')
           ) // check the surrounding squares; if filled/off-field, fill this one
          ffield[FIELD_WIDTH * y + x] = '-'; // dash is not zero
    for (unsigned rindex = 0; rindex < pdefs[piece][0].rstates; ++rindex)
      for (unsigned x = 0; x < pdefs[piece][rindex].width; ++x)
      {
        unsigned height = 0;
        for (unsigned y = 0; y < 4; ++y) // find the bottom of this column
          if (pdefs[piece][rindex].def[(4 * y) + x]) // TODO: unhardcode
            height = y;
        for (unsigned xf = x; xf < FIELD_WIDTH; ++xf) // loop across columns
        {
          unsigned fieldHeight = columnHeight(&ffield, xf); // find height of this column
          PieceLocation found = PieceLocation { xf - x, FIELD_HEIGHT - (fieldHeight + height), rindex }; // calculate location (x, y, r, rank)
          bool invalid = false;
          for (unsigned w = 0; w < (4 * 4); ++w) // loop across spaces in def
          {
            unsigned fx = found.x + (w % 4);
            unsigned fy = found.y + (w / 4);
            if (pdefs[piece][found.r].def[w] && !(fy < FIELD_HEIGHT && fx < FIELD_WIDTH && ffield[(FIELD_WIDTH * fy) + fx] == '0'))
            {
              // if piece defined at this square, and it's either off the field, or overlaps
              invalid = true; // it's no good
              break; // we can stop looking at it
            }
          }
          if (!invalid)
          {
            found.rank = rank(piece, found); // rank it
            placements.push_back(found); // list it
          }
        }
      }

    if (placements.size() == 0)
    {
      LOG4CXX_INFO(logger, "No more moves!");
      break;
    }
    PieceLocation theChosenOne = *max_element(placements.begin(), placements.end(), [](PieceLocation i, PieceLocation j) { return i.rank < j.rank; });
    int col = (rand() % 4) + 1; // random color
    place(field, piece, theChosenOne, col);
    LOG4CXX_TRACE(logger, "Placing x: " << theChosenOne.x << " y: " << theChosenOne.y);
    int crow = clearCount(&ffield);
    SendCommand(TetrinetMessage::F, string(field.begin(), field.end()));
    manager->statusHandler({ *GetName(), field });
    fmtx.unlock();
    try
    {
      this_thread::sleep(pieceDelay);
    }
    catch(thread_interrupted&)
    {
      break;
    }
  }
}

inline double AIEngine::rank(int piece, PieceLocation location)
{
  vector<char> newfield = vector<char>(field);
  place(newfield, piece, location, 1);
  return
      ((double)(clearCount   (&newfield)                        ) * _c)
    + ((double)(blockadeCount(&newfield) - blockadeCount(&field)) * _b)
    + ((double)(rowCount     (&newfield) - rowCount     (&field)) * _r)
    + ((double)(gapCount     (&newfield) - gapCount     (&field)) * _g);
}

inline unsigned AIEngine::columnHeight(const vector<char>* _field, unsigned x)
{
  unsigned colHeight = FIELD_HEIGHT - 1;
  for (unsigned int y = 0; y < FIELD_HEIGHT; ++y)  // Search this column top-down
    if ((*_field)[FIELD_WIDTH * y + x] != '0') // block!
    {
      colHeight = y; // this is the top
      break;
    }
  return FIELD_HEIGHT - colHeight;
}

inline int AIEngine::gapCount(const vector<char>* _field)
{
  unsigned gapCount = 0;
  for (unsigned x = 0; x < FIELD_WIDTH; ++x)
  {
    bool foundBlock = false;
    for (unsigned y = 0; y < FIELD_HEIGHT; ++y) // Search this column top-down
    {
      if ((*_field)[FIELD_WIDTH * y + x] != '0') // if there's a block
        foundBlock = true; // flag it
      else if (foundBlock) // if there's no block and we've flagged
        ++gapCount;
    }
  }
  return gapCount;
}

inline int AIEngine::blockadeCount(const vector<char>* _field)
{
  unsigned totalBlock = 0;
  for (unsigned x = 0; x < FIELD_WIDTH; ++x)
  {
    unsigned blockCount = 0;
    bool foundBlock = false;
    for (unsigned y = 0; y < FIELD_HEIGHT; ++y) // Search this column top-down
    {
      if ((*_field)[FIELD_WIDTH * y + x] != '0') // if there's a block
      {
        foundBlock = true; // flag it
        blockCount++;
      }
      else if (foundBlock) // if we have blocks over us and this is a gap, stop counting
        goto legitimateUseOfGoto;
    }
    blockCount = 0; // reached the bottom of col. without tripping goto, full col., don't count
legitimateUseOfGoto:
    totalBlock += blockCount;
  }
  return totalBlock;
}

inline int AIEngine::rowCount(const vector<char>* _field)
{
  vector<unsigned>* heights = new vector<unsigned>();
  for (unsigned x = 0; x < FIELD_WIDTH; ++x)
    heights->push_back(columnHeight(_field, x));
  return FIELD_HEIGHT - *std::min_element(heights->begin(), heights->end()); // find the tallest column
}

inline int AIEngine::clearCount(vector<char>* _field)
{
  int clears = 0, crow = 0; // https://www.youtube.com/watch?v=jYmn3Gwn3oI
  for (long unsigned i = 0; i < _field->size(); ++i)
  {
    if ((*_field)[i] != '0') ++crow;
    if ((i % FIELD_WIDTH) == FIELD_WIDTH - 1)
    {
      if (crow == FIELD_WIDTH)
      {
        for (long unsigned s = (i / FIELD_WIDTH) * FIELD_WIDTH; s <= i; ++s)
          (*_field)[s] = '0';
        for (long unsigned s = i; s > 0; --s)
          (*_field)[s] = s > FIELD_WIDTH ? (*_field)[s - FIELD_WIDTH] : '0';
        clears++;
      }
      crow = 0;
    }
  }
  return clears;
}

inline void AIEngine::place(vector<char>& _field, unsigned piece, PieceLocation location, unsigned col)
{
  for (unsigned w = 0; w < 4 * 4; w++) // copy piece over
    if (pdefs[piece][location.r].def[w]) // if placing piece is defined here
      _field.at((((w / 4) + location.y) * FIELD_WIDTH) + ((w % 4) + location.x)) = col + 48; // color to ascii character
}

void AIEngine::ProcessCommand(TetrinetMessage message, deque<string>* tokens)
{
  try
  {
    TetrinetClient::ProcessCommand(message, tokens);
    switch (message)
    {
    case TetrinetMessage::PLINEACT:
    case TetrinetMessage::PLINE:
    {
      stringstream msg;
      for (long unsigned i = 1; i < tokens->size(); ++i)
        msg << tokens->at(i) << " ";
      LOG4CXX_INFO(logger, "MSG: " << msg.str());
    }
      break;

    case TetrinetMessage::F:
      if (atoi(tokens->at(0).c_str()) == *GetID())
      {
        fmtx.lock();
        field.assign(tokens->at(1).begin(), tokens->at(1).end());
        fmtx.unlock();
      }
      break;

    case TetrinetMessage::NEWGAME:
    {
      string freqs = tokens->at(7);
      string specs = tokens->at(8);
      for (unsigned int i = 0; i < freqs.length(); ++i)
        freqarr.insert(freqarr.end(), atoi(freqs.substr(i, 1).c_str()));
      for (unsigned int i = 0; i < specs.length(); ++i)
        specarr.insert(specarr.end(), atoi(specs.substr(i, 1).c_str()));
      stringstream seedstr;
      seedstr << dec << tokens->at(11);
      srand(atoi(seedstr.str().c_str())); //TODO
      fill(field.begin(), field.end(), '0');
      placer = new thread(&AIEngine::DoPlacing, this);
    }
    break;

    case TetrinetMessage::ENDGAME:
      Stop();
      break;
    }
  }
  catch (out_of_range& e)
  {
  }
}
