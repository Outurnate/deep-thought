/*
 * AIEngine.cpp
 *
 *  Created on: Oct 26, 2012
 *      Author: joseph
 */

#include <cctype>
#include <vector>
#include <iomanip>
#include <sstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "AIEngine.hpp"

#include "Constants.hpp"

using boost::asio::ip::tcp;

using namespace std;
using namespace log4cxx;
using namespace boost::algorithm;

// Hashes of the message tokens
#define TOKEN_F                     2654435871
#define TOKEN_TEAM             706246331656118
#define TOKEN_PLINE          46082575837886005
#define TOKEN_NEWGAME      6807035119030705139
#define TOKEN_ENDGAME      6807153467799589089
#define TOKEN_WINLIST      6807035647241175774
#define TOKEN_PLAYERNUM    1514265613367712513
#define TOKEN_PLAYERJOIN   5995087092028005159
#define TOKEN_PLAYERLEAVE 12876743988251149938

AIEngine::AIEngine(string nickname, double g, double b, double r, double c, AIManager* manager, LoggerPtr logger)
  : SCREEN_NAME(nickname), string_hash(), plyrids(), freqarr(), specarr(), field(FIELD_SIZE, '0'), playernum(0), _g(g), _b(b), _r(r), _c(c), cancelPlacing(false),
    service(), socket(service), socio(service), fmtx(), pieceDelay(1), pdefs(), manager(manager), logger(logger)
{
  plyrids.insert(pair<int, string>(0, "Server"));
  pdefs = vector<vector<PieceDef> >
  {
    vector<PieceDef>
    {
      // I
      PieceDef { vector<bool> { false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true }, 4, 1 },
      PieceDef { vector<bool> { true, false, false, false, true, false, false, false, true, false, false, false, true, false, false, false }, 1, 4 }
    },
    vector<PieceDef>
    {
      // O
      PieceDef { vector<bool> { false, false, false, false, false, false, false, false, true, true, false, false, true, true, false, false }, 2, 2 }
    },
    vector<PieceDef>
    {
      // J
      PieceDef { vector<bool> { false, false, false, false, false, false, false, false, true, false, false, false, true, true, true, false }, 3, 2 },
      PieceDef { vector<bool> { false, false, false, false, false, true, false, false, false, true, false, false, true, true, false, false }, 2, 3 },
      PieceDef { vector<bool> { false, false, false, false, false, false, false, false, true, true, true, false, false, false, true, false }, 3, 2 },
      PieceDef { vector<bool> { false, false, false, false, true, true, false, false, true, false, false, false, true, false, false, false }, 2, 3 }
    },
    vector<PieceDef>
    {
      // L
      PieceDef { vector<bool> { false, false, false, false, false, false, false, false, true, true, true, false, true, false, false, false }, 3, 2 },
      PieceDef { vector<bool> { false, false, false, false, true, false, false, false, true, false, false, false, true, true, false, false }, 2, 3 },
      PieceDef { vector<bool> { false, false, false, false, false, false, false, false, false, false, true, false, true, true, true, false }, 3, 2 },
      PieceDef { vector<bool> { false, false, false, false, true, true, false, false, false, true, false, false, false, true, false, false }, 2, 3 }
    },
    vector<PieceDef>
    {
      // S
      PieceDef { vector<bool> { false, false, false, false, false, false, false, false, false, true, true, false, true, true, false, false }, 3, 2 },
      PieceDef { vector<bool> { false, false, false, false, true, false, false, false, true, true, false, false, false, true, false, false }, 2, 3 }
    },
    vector<PieceDef>
    {
      // Z
      PieceDef { vector<bool> { false, false, false, false, false, false, false, false, true, true, false, false, false, true, true, false }, 3, 2 },
      PieceDef { vector<bool> { false, false, false, false, false, true, false, false, true, true, false, false, true, false, false, false }, 2, 3 }
    },
    vector<PieceDef>
    {
      // T
      PieceDef { vector<bool> { false, false, false, false, false, false, false, false, false, true, false, false, true, true, true, false }, 3, 2 },
      PieceDef { vector<bool> { false, false, false, false, false, true, false, false, true, true, false, false, false, true, false, false }, 2, 3 },
      PieceDef { vector<bool> { false, false, false, false, false, false, false, false, true, true, true, false, false, true, false, false }, 3, 2 },
      PieceDef { vector<bool> { false, false, false, false, true, false, false, false, true, true, false, false, true, false, false, false }, 2, 3 }
    }
  }; // pdefs[tetramino][orientation];
}

AIEngine::AIEngine(const AIEngine &engine)
  : service(), socket(service), socio(service), pieceDelay(engine.pieceDelay.total_seconds())
{
  string_hash = engine.string_hash;
  plyrids = engine.plyrids;
  freqarr = engine.freqarr;
  specarr = engine.specarr;
  field = engine.field;
  playernum = engine.playernum;
  _g = engine._g;
  _b = engine._b;
  _r = engine._r;
  _c = engine._c;
  cancelPlacing = engine.cancelPlacing;
  pdefs = engine.pdefs;
  logger = engine.logger;
}

AIEngine::~AIEngine()
{
  //TODO: close socket and io_service
}

inline string AIEngine::makeHex(int dec)
{
  stringstream decstr;
  decstr << setfill('0') << setw(2) << hex << dec;
  return decstr.str();
}

inline string AIEngine::encode(string name, int ip[])
{
  string stmsg = "tetrisstart " + name + " 1.13";
  stringstream sipstr;
  sipstr << 54 * ip[0] + 41 * ip[1] + 29 * ip[2] + 17 * ip[3];
  string ipmsg = sipstr.str();
  int j = 128;
  string result = makeHex(j);
  for (unsigned int i = 0; i < stmsg.length(); ++i)
  {
    j = ((j + stmsg[i]) % 255) ^ ipmsg[i % ipmsg.length()];
    result += makeHex(j);
  }
  return result;
}

void AIEngine::Run()
{
  try
  {
    int ip[] = { 127, 0, 0, 1 };
    tcp::resolver resolver(socio);
    tcp::resolver::query query(tcp::v4(), "127.0.0.1", "31457");
    tcp::resolver::iterator iterator = resolver.resolve(query);
    string startmsg = encode(SCREEN_NAME, ip) + "\n";
    boost::asio::connect(socket, iterator);
    boost::asio::write(socket, boost::asio::buffer(startmsg, 1024));
    for (;;)
    {
      boost::asio::streambuf response;
      boost::asio::read_until(socket, response, "\xFF");
      stringstream strres;
      strres << (boost::asio::streambuf*)&response;
      string res = strres.str();
      vector<string> mshbits;
      split(mshbits, res, is_any_of("\xFF"));
      for (unsigned int i = 0; i < mshbits.size(); ++i)
        this->ProcessCommand(mshbits[i]);
    }
  }
  catch (exception& e)
  {
    LOG4CXX_FATAL(logger, "Fatal error in connect: " << e.what());
  }
}

void AIEngine::DoPlacing()
{
  while (!this->cancelPlacing)
  {
    boost::this_thread::sleep(pieceDelay);
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
    for (unsigned rindex = 0; rindex < pdefs[piece].size(); ++rindex)
      for (unsigned x = 0; x < pdefs[piece][rindex].width; ++x)
      {
        unsigned height = 0;
        for (unsigned y = 0; y < 4; ++y) // find the bottom of this column
          if (pdefs[piece][rindex].def[(4 * y) + x]) // TODO: unhardcode
            height = y;
        for (unsigned xf = x; xf < (FIELD_WIDTH - (pdefs[piece][rindex].width - x) - 1); ++xf) // loop across columns
        {
          unsigned fieldHeight = columnHeight(&ffield, xf); // find height of this column
          PieceLocation found;
          found.x = xf - x;
          found.y = FIELD_HEIGHT - (fieldHeight + height) - 1;
          found.r = rindex; // calculate location (x, y, r, rank)
          bool invalid = false;
          for (unsigned w = 0; w < pdefs[piece][rindex].width * pdefs[piece][rindex].height; ++w) // loop across spaces in def
          {
            unsigned fx = found.x + (w % pdefs[piece][rindex].width);
            unsigned fy = found.y + (w / pdefs[piece][rindex].width);
            if (pdefs[piece][found.r].def[w] && !((fy < FIELD_HEIGHT && ffield[(FIELD_WIDTH * fy) + fx] == '0')))
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
    LOG4CXX_TRACE(logger, "Placing x:" << theChosenOne.x << " y:" << theChosenOne.y);
    int crow = 0;
    for (long unsigned i = 0; i < field.size(); ++i) // loop over field, find full rows and remove them
    {
      if (field[i] != '0') ++crow; // increment for each block
      if ((i % FIELD_WIDTH) == FIELD_WIDTH - 1) // on the last block
      {
        if (crow == FIELD_WIDTH) // full row?
        {
          for (long unsigned int s = (i / FIELD_WIDTH) * FIELD_WIDTH; s <= i; ++s) // clear
            field[s] = '0';
          for (long unsigned int s = i; s > 0; --s) // drop
            field[s] = s > FIELD_WIDTH ? field[s - FIELD_WIDTH] : '0';
        }
        crow = 0;
      }
    }
    stringstream fstrs;
    field[0] = '5';
    fstrs << "f " << playernum << " " << string(field.begin(), field.end()) << "\xFF";
    string fstr = fstrs.str();
    boost::asio::write(socket, boost::asio::buffer(string(fstr.begin(), fstr.end()), 1024));
    manager->statusHandler({ SCREEN_NAME, field });
    fmtx.unlock();
  }
}

void AIEngine::Stop()
{
  cancelPlacing = true;
  // TODO: stop....
}

inline double AIEngine::rank(int piece, PieceLocation location)
{
  vector<char>* newfield = new vector<char>(field);
  place(*newfield, piece, location, 1);
  return
      ((double)(clearCount   (*newfield) - clearCount   (field)) * _c)
    + ((double)(blockadeCount(newfield) - blockadeCount(&field)) * _b)
    + ((double)(rowCount     (newfield) - rowCount     (&field)) * _r)
    + ((double)(gapCount     (newfield) - gapCount     (&field)) * _g);
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

inline int AIEngine::clearCount(vector<char> _field)
{
  int clears = 0, crow = 0; // https://www.youtube.com/watch?v=jYmn3Gwn3oI
  for (long unsigned i = 0; i < _field.size(); ++i)
  {
    if (_field[i] != '0') ++crow;
    if ((i % FIELD_WIDTH) == FIELD_WIDTH - 1)
    {
      if (crow == FIELD_WIDTH)
      {
        for (long unsigned s = (i / FIELD_WIDTH) * FIELD_WIDTH; s <= i; ++s)
          _field[s] = '0';
        for (long unsigned s = i; s > 0; --s)
          _field[s] = s > FIELD_WIDTH ? _field[s - FIELD_WIDTH] : '0';
        clears++;
      }
      crow = 0;
    }
  }
  return clears;
}

inline void AIEngine::place(vector<char>& _field, unsigned piece, PieceLocation location, unsigned col)
{
  LOG4CXX_TRACE(logger, string(_field.begin(), _field.end()));
  for (unsigned w = 0; w < pdefs[piece][location.r].width * pdefs[piece][location.r].height; w++) // copy piece over
    if (pdefs[piece][location.r].def[w]) // if placing piece is defined here
      _field[(FIELD_WIDTH * location.y) + location.x + (w % pdefs[piece][location.r].width) + ((w / pdefs[piece][location.r].width) * FIELD_WIDTH)] = col + 48; // color to ascii character
}

inline string AIEngine::cleanCodes(string orig)
{
  vector<char> newV(orig.length()), oldV(orig.begin(), orig.end());
  remove_copy_if(oldV.begin(), oldV.end(), newV.begin(), [](char c) { return iscntrl(c); });
  return string(newV.begin(), newV.end());
}

void AIEngine::ProcessCommand(string cmd)
{
  vector<string> tokens;
  split(tokens, cmd, is_any_of(" "));
  long unsigned size = tokens.size();
  if (size == 0)
    return;
  try
  {
    switch (string_hash(tokens[0]))
    {
    case TOKEN_PLINE:
    {
      stringstream msg;
      for (long unsigned i = 2; i < size; ++i)
        msg << cleanCodes(tokens.at(i)) << " ";
      LOG4CXX_INFO(logger, "MSG: " << cleanCodes(msg.str()));
    }
      break;

    case TOKEN_PLAYERNUM:
      plyrids.insert(pair<int, string>(playernum = atoi(tokens.at(1).substr(0, 1).c_str()), "Me"));
      break;

    case TOKEN_F:
      if (atoi(tokens.at(1).c_str()) == playernum)
      {
        fmtx.lock();
        field.assign(tokens.at(2).begin(), tokens.at(2).end());
        fmtx.unlock();
      }
      break;

    case TOKEN_NEWGAME:
    {
      string freqs = tokens.at(8);
      string specs = tokens.at(9);
      for (unsigned int i = 0; i < freqs.length(); ++i)
        freqarr.insert(freqarr.end(), atoi(freqs.substr(i, 1).c_str()));
      for (unsigned int i = 0; i < specs.length(); ++i)
        specarr.insert(specarr.end(), atoi(specs.substr(i, 1).c_str()));
      stringstream seedstr;
      seedstr << dec << tokens.at(12);
      srand(atoi(seedstr.str().c_str()));
      cancelPlacing = false;
      fill(field.begin(), field.end(), '0');
      boost::thread placer(&AIEngine::DoPlacing, this);
    }
    break;

    case TOKEN_ENDGAME:
      cancelPlacing = true;
      break;
    }
  }
  catch (out_of_range& e)
  {
  }
}
