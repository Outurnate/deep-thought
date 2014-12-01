/*
 * AIEngine.cpp
 *
 *  Created on: Oct 26, 2012
 *      Author: joseph
 */

#include <vector>
#include <iomanip>
#include <sstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "AIEngine.hpp"

#include "Constants.hpp"

using boost::asio::ip::tcp;
using namespace std;
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

AIEngine::AIEngine(string nickname, double g, double b, double r, double c)
  : SCREEN_NAME(nickname), string_hash(), plyrids(), freqarr(), specarr(), field(FIELD_SIZE, '0'), playernum(0), _g(g), _b(b), _r(r), _c(c), cancelPlacing(false),
    service(), socket(service), socio(service), fmtx(), pieceDelay(1), pdefs()
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
    cout << e.what();
  }
}

void AIEngine::DoPlacing()
{
  while (!this->cancelPlacing)
  {
    boost::this_thread::sleep(pieceDelay);
    int piece = freqarr[rand() % 100] - 1; // not perfect, but, meh.

    fmtx.lock();
    vector<PieceLocation> pos; // vector for all locations
    for (long unsigned int r = 0; r < pdefs[piece].size(); ++r)
      for (unsigned int x = 0; x < (unsigned int)(FIELD_WIDTH - pdefs[piece][r].width + 1); ++x)
      {
        vector<int> yyz;
        for (unsigned int x2 = x; x2 < x + pdefs[piece][r].width; ++x2)
        {
          int ly = FIELD_HEIGHT;
          for (unsigned int y = 1; y < FIELD_HEIGHT; ++y)
            if (field[FIELD_WIDTH * y + x2] != '0')
            {
              ly = y;
              break;
            }
          if (ly == 1 && (piece == 0 || piece == 1))
	    goto top_out;
          yyz.push_back(ly - 4);
        }
        {
          int y = *min_element(yyz.begin(), yyz.end());
          if (piece == 2 || piece == 3)
          {
            pos.push_back(PieceLocation { x, y + 1, r });
            pos.push_back(PieceLocation { x, y + 2, r });
          }
          else if (piece >= 4 || piece <= 6)
          {
            pos.push_back(PieceLocation { x, y + 1, r });
          }
          pos.push_back(PieceLocation { x, y, r });
        }
        top_out:;
      }
    vector<AdvancedPieceLocation> clean; // filter positions
    for (long unsigned int i = 0; i < pos.size(); ++i)
    {
      bool isUnclean = false;
      try
      {
        for (int w = 0; w < 4 * 4; ++w)
          if (pdefs[piece][pos[i].r].def[w])
            isUnclean |= field.at((FIELD_WIDTH * pos[i].y) + pos[i].x + (w % 4) + ((w / 4) * FIELD_WIDTH)) != '0';
        if (isUnclean)
          cout << "Rejected Location: (" << pos[i].x << ", " << pos[i].y << "), " << pos[i].r << ".\tReason: Block overlap." << endl;
      }
      catch (out_of_range& e)
      {
        isUnclean = true;
        cout << "Rejected Location: (" << pos[i].x << ", " << pos[i].y << "), " << pos[i].r << ".\tReason: Off the field." << endl;
      }
      if (!isUnclean)
        clean.push_back(AdvancedPieceLocation { pos[i], isUnclean ? 0.f : rank(piece, pos[i]) , !isUnclean });
    }
    std::cout << clean.size() << std::endl;
    long unsigned int index = 0;
    double hr = -numeric_limits<double>::max();
    for (long unsigned int i = 0; i < clean.size(); ++i)
      if (clean[i].valid && clean[i].rank > hr)
      {
        index = i;
        hr = clean[i].rank;
      }
    int col = (rand() % 4) + 1;
    for (int w = 0; w < 4 * 4; w++)
      if (pdefs[piece][clean[index].basic.r].def[w])
      {
        char buf[2];
        sprintf(buf, "%d", col);
        field[(FIELD_WIDTH * clean[index].basic.y) + clean[index].basic.x + (w % 4) + ((w / 4) * FIELD_WIDTH)] = buf[0];
      }
    int crow;
    for (long unsigned int i = 0; i < field.size(); ++i)
    {
      if (field[i] != '0') ++crow;
      if ((i % FIELD_WIDTH) == FIELD_WIDTH - 1)
      {
        if (crow == FIELD_WIDTH)
        {
          for (long unsigned int s = (i / FIELD_WIDTH) * FIELD_WIDTH; s <= i; ++s)
            field[s] = '0';
          for (long unsigned int s = i; s > 0; --s)
            field[s] = s > FIELD_WIDTH ? field[s - FIELD_WIDTH] : '0';
        }
        crow = 0;
      }
    }
    stringstream fstrs;
    fstrs << "f " << playernum << " " << string(field.begin(), field.end()) << "\xFF";
    string fstr = fstrs.str();
    boost::asio::write(socket, boost::asio::buffer(string(fstr.begin(), fstr.end()), 1024));
    fmtx.unlock();
  }
}

inline double AIEngine::rank(int piece, PieceLocation location)
{
  vector<char> newfield(field);
  for (int w = 0; w < 4 * 4; ++w)
    if (pdefs[piece][location.r].def[w])
      newfield[(FIELD_WIDTH * location.y) + location.x + (w % 4) + ((w / 4) * FIELD_WIDTH)] = '1';
  return 
    ((double)(clearCount   (newfield) - clearCount   (field)) * _c)
    + ((double)(blockadeCount(newfield) - blockadeCount(field)) * _b)
    + ((double)(rowCount     (newfield) - rowCount     (field)) * _r)
    + ((double)(gapCount     (newfield) - gapCount     (field)) * _g);
}

inline int AIEngine::gapCount(const std::vector<char>& _field)
{
  unsigned gapCount = 0;
  for (unsigned int x = 0; x < FIELD_WIDTH; ++x)
  {
    bool foundBlock = false;
    for (unsigned int y = 1; y < FIELD_HEIGHT; ++y) // Search this column top-down
    {
      if (_field[FIELD_WIDTH * y + x] != '0') // if there's a block
        foundBlock = true; // flag it
      else if (foundBlock) // if there's no block and we've flagged
        ++gapCount;
    }
  }
  return gapCount;
}

inline int AIEngine::blockadeCount(const std::vector<char>& _field)
{
  unsigned totalBlock = 0;
  for (unsigned int x = 0; x < FIELD_WIDTH; ++x)
  {
    unsigned blockCount = 0;
    bool foundBlock = false;
    for (unsigned int y = 1; y < FIELD_HEIGHT; ++y) // Search this column top-down
    {
      if (_field[FIELD_WIDTH * y + x] != '0') // if there's a block
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

inline int AIEngine::rowCount(const std::vector<char>& _field)
{
  std::vector<unsigned> heights;
  for (unsigned int x = 0; x < FIELD_WIDTH; ++x)
  {
    unsigned colHeight = 0;
    for (unsigned int y = 1; y < FIELD_HEIGHT; ++y)  // Search this column top-down
      if (_field[FIELD_WIDTH * y + x] != '0') // block!
      {
	colHeight = y; // this is the top
	break;
      }
    heights.push_back(colHeight);
  }
  return FIELD_HEIGHT - *std::min_element(heights.begin(), heights.end()); // find the tallest column
}

inline int AIEngine::clearCount(std::vector<char>& _field)
{
  int clears = 0, crow;
  for (long unsigned int i = 0; i < _field.size(); ++i)
  {
    if (field[i] != '0') ++crow;
    if ((i % FIELD_WIDTH) == FIELD_WIDTH - 1)
    {
      if (crow == FIELD_WIDTH)
      {
        for (long unsigned int s = (i / FIELD_WIDTH) * FIELD_WIDTH; s <= i; ++s)
          _field[s] = '0';
        for (long unsigned int s = i; s > 0; --s)
          _field[s] = s > FIELD_WIDTH ? _field[s - FIELD_WIDTH] : '0';
        clears++;
      }
      crow = 0;
    }
  }
  return clears;
}

void AIEngine::ProcessCommand(string cmd)
{
  vector<string> tokens;
  split(tokens, cmd, is_any_of(" "));
  long unsigned int size = tokens.size();
  if (size == 0)
    return;
  //cout << "!!!" << string_hash(tokens[0]) << "!!!" << tokens[0] << endl;
  try
  {
    switch (string_hash(tokens[0]))
    {
    case TOKEN_PLINE:
      for (long unsigned int i = 2; i < size; ++i)
        cout << tokens.at(i) << " ";
      cout << endl;
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
