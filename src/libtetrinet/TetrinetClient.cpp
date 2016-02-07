#include "libtetrinet/TetrinetClient.hpp"

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "CompilerMagic.hpp"
#include "libtetrinet/PieceShape.hpp" // TODO REMOVE
#include "libtetrinet/PieceRotation.hpp" // TODO REMOVE
#include "libtetrinet/TetrinetMessage.hpp"

using boost::asio::ip::tcp;

using namespace std;
using namespace boost;
using namespace boost::chrono;
using namespace boost::algorithm;
using namespace boost::system;
using namespace boost::asio;
using namespace log4cxx;

TetrinetClient::TetrinetClient(string nickname, LoggerPtr logger) : service(), socket(), screenName(nickname), logger(logger), playerNum(0), connected(false), inGame(false), paused(false), eval(logger)
{
}

string makeHex(int dec)
{
  stringstream decstr;
  decstr << setfill('0') << setw(2) << hex << dec;
  return decstr.str();
}

inline string TetrinetClient::encode(string name, string address)
{
  vector<string> ip;
  split(ip, address, is_any_of("."));
  string stmsg = "tetrisstart " + name + " 1.13";
  stringstream sipstr;
  sipstr << 54 * atoi(ip[0].c_str()) + 41 * atoi(ip[1].c_str()) + 29 * atoi(ip[2].c_str()) + 17 * atoi(ip[3].c_str());
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

void TetrinetClient::Run(std::shared_ptr<boost::asio::io_service> service, TetrinetConnection& connection)
{
  this->service = service;
  this->socket.reset(new tcp::socket(*service));
  try
  {
    tcp::resolver resolver(*service);
    tcp::resolver::query query(tcp::v4(), "127.0.0.1", "31457"); // limit to ipv4...since protocol doesn't support ipv6
    tcp::resolver::iterator iterator = resolver.resolve(query), end;
    boost::system::error_code errorc = error::host_not_found;
    while (errorc && iterator != end)
    {
      socket->close();
      socket->connect(*iterator++, errorc);
    }
    if (errorc)
      throw errorc;
    string startmsg = encode(screenName, socket->remote_endpoint().address().to_v4().to_string()) + "\n";
    write(*socket, buffer(startmsg, startmsg.length()));
    connected = true;
    onConnect(*this);
    while (connected)
    {
      boost::asio::streambuf response;
      read_until(*socket, response, "\xFF");
      stringstream strres;
      strres << (boost::asio::streambuf*)&response;
      string res = strres.str();
      vector<string> mshbits;
      split(mshbits, res, is_any_of("\xFF"));
      for (unsigned int i = 0; i < mshbits.size(); ++i)
      {
        deque<string> tokens;
        split(tokens, mshbits[i], is_any_of(" "));
        if (tokens.size() == 0)
          break;
        TetrinetMessage hash = TetrinetMessage(djb2(stringToUpper(tokens.front()).c_str()));
	cout << tokens.at(0) << endl;
        tokens.pop_front();
        this->processCommand(hash, tokens);
      }
    }
  }
  catch (boost::system::error_code& e)
  {
    LOG4CXX_FATAL(logger, "Fatal error in connect: " << e.message());
    Stop();
  }
}

void TetrinetClient::Stop()
{
  connected = false;
  paused = false;
  inGame = false;
  onDisconnect(*this);
}

void TetrinetClient::placer()
{
  if (!gameData)
  {
    // wait for data
    cout << "nnnn" << endl;
  }
  while (inGame && !paused)
  {
    this_thread::sleep_for(seconds(1));
    PieceLocation newPiece(NewPiece(gameData.get().GetPiece()));
    LOG4CXX_TRACE(logger, "Placing x=" << newPiece.GetX() << ",y=" << newPiece.GetY());
    players[playerNum.get()]->field.ApplyTransform(newPiece);
    FieldTransform clear;
    GetEvaluator().ClearCount(GetField(), clear);
    players[playerNum.get()]->field.ApplyTransform(clear);
    sendCommand(TetrinetMessage::F, players[playerNum.get()]->field);
  }
}

void TetrinetClient::sendCommand(TetrinetMessage message, string param)
{
  stringstream str;
  str << GetMessageString(message) << " ";
  cout << str.str() << endl;
  switch(message)
  {
  case TetrinetMessage::F:
    str << playerNum.get() << " " << param << " ";
    break;
  default:
    str << param;
    break;
  }
  str << "\xFF";
  string sstr = str.str();
  write(*socket, buffer(string(sstr.begin(), sstr.end()), sstr.length()));
}

void TetrinetClient::processCommand(TetrinetMessage message, deque<string>& tokens)
{
  try
  {
    switch(message)
    {
    case TetrinetMessage::WINLIST:
      break;
    
    case TetrinetMessage::PLAYERWON:
      onPlayerWin(*this);
      break;
    
    case TetrinetMessage::PLAYERLOST:
      onPlayerLost(*this);
      break;
    
    case TetrinetMessage::SB:
      onSpecial(*this);
      break;
    
    case TetrinetMessage::LVL:
      players[lexical_cast<unsigned>(tokens.at(0))]->level = lexical_cast<unsigned>(tokens.at(1));
      break;

    case TetrinetMessage::TEAM:
      players[lexical_cast<unsigned>(tokens.at(0))]->team = tokens.at(1);
      break;

    case TetrinetMessage::GMSG:
    case TetrinetMessage::PLINE:
    case TetrinetMessage::PLINEACT:
      onMessage(*this);
      break;
    
    case TetrinetMessage::PLAYERNUM:
      playerNum = lexical_cast<unsigned>(tokens.at(0).substr(0, 1));
      setField(playerNum.get(), screenName);
      break;
      
    case TetrinetMessage::PLAYERJOIN:
      if (playerNum)
	LOG4CXX_WARN(logger, "Server sent playernum then playerjoin for us.  Please report to server owner");
      setField(lexical_cast<unsigned>(tokens.at(0).substr(0, 1)), tokens.at(1));
      onPlayerJoin(*this);
      break;
      
    case TetrinetMessage::PLAYERLEAVE:
      players[lexical_cast<unsigned>(tokens.at(0).substr(0, 1))].reset();
      onPlayerLeave(*this);
      break;
      
    case TetrinetMessage::NOCONNECTING:
      LOG4CXX_ERROR(logger, "Noconnect received: " << tokens.at(0));
      Stop();
      break;

    case TetrinetMessage::F:
      players[lexical_cast<unsigned>(tokens.at(0))]->field.ApplyTransform(FieldTransform(tokens.at(1)));
      onField(*this);
      break;
    
    case TetrinetMessage::NEWGAME:
      cout << "aaaa" << endl;
      cout << tokens.size() << endl;
      gameData = GameSettings(lexical_cast<unsigned>(tokens.at(0)), lexical_cast<unsigned>(tokens.at(1)), lexical_cast<unsigned>(tokens.at(2)),
			      lexical_cast<unsigned>(tokens.at(3)), lexical_cast<unsigned>(tokens.at(4)), lexical_cast<unsigned>(tokens.at(5)),
			      lexical_cast<unsigned>(tokens.at(6)), tokens.at(7), tokens.at(8), lexical_cast<bool>(tokens.at(9)),
			      lexical_cast<bool>(tokens.at(10)), stol(tokens.at(11), 0, 16)); // last param is 1.14 only TODO
      LOG4CXX_INFO(logger, "Game started");
      players[playerNum.get()]->field.Reset();
      if (!gameThread)
	gameThread = thread(&TetrinetClient::placer, this);
      inGame = true;
      onGameStart(*this);
      break;
      
    case TetrinetMessage::INGAME:
      inGame = true;
      break;
    
    case TetrinetMessage::PAUSE:
      if ((paused = lexical_cast<bool>(tokens.at(0))))
	onPause(*this);
      else
	onResume(*this);
      break;

    case TetrinetMessage::ENDGAME:
      gameData = none;
      inGame = false;
      onGameEnd(*this);
      break;
    }
  }
  catch (std::out_of_range)
  {
  }
  catch (boost::bad_lexical_cast& e)
  {
    cout << e.what() << endl;
  }
}

void TetrinetClient::setField(unsigned num, const string& name)
{
  if (players[num])
  {
    LOG4CXX_WARN(logger, "Field leaked!");
    players[num].reset();
  }
  players[num].reset(new TetrinetPlayer(name, ""));
}

const string TetrinetClient::GetName() const
{
  return screenName;
}

int TetrinetClient::GetID() const
{
  return playerNum.get();
}

const Field& TetrinetClient::GetField() const
{
  return players.at(playerNum.get())->field;
}

const FieldEvaluator& TetrinetClient::GetEvaluator() const
{
  return eval;
}
