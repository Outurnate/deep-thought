#include "TetrinetClient.hpp"

#include <string>
#include <sstream>
#include <iomanip>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using boost::asio::ip::tcp;

using namespace std;
using namespace boost;
using namespace boost::algorithm;
using namespace boost::system;
using namespace boost::asio;
using namespace log4cxx;

TetrinetClient::TetrinetClient(string nickname, LoggerPtr logger) : screenName(nickname), logger(logger), playernum(0), service(), socket(service), socio(service)
{
  plyrids.insert(pair<int, string>(0, "Server"));
}

TetrinetClient::TetrinetClient(const TetrinetClient& client) : service(), socket(service), socio(service)
{
}

TetrinetClient::~TetrinetClient()
{
}

inline string TetrinetClient::makeHex(int dec)
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

void TetrinetClient::Run()
{
  try
  {
    tcp::resolver resolver(socio);
    tcp::resolver::query query(tcp::v4(), "127.0.0.1", "31457"); // limit to ipv4...since protocol doesn't support ipv6
    tcp::resolver::iterator iterator = resolver.resolve(query), end;
    boost::system::error_code errorc = error::host_not_found;
    while (errorc && iterator != end)
    {
      socket.close();
      socket.connect(*iterator++, errorc);
    }
    if (errorc)
      throw errorc;
    string startmsg = encode(screenName, socket.remote_endpoint().address().to_v4().to_string()) + "\n";
    write(socket, buffer(startmsg, 1024));
    for (;;)
    {
      boost::asio::streambuf response;
      read_until(socket, response, "\xFF");
      stringstream strres;
      strres << (boost::asio::streambuf*)&response;
      string res = strres.str();
      vector<string> mshbits;
      split(mshbits, res, is_any_of("\xFF"));
      for (unsigned int i = 0; i < mshbits.size(); ++i)
      {
        deque<string> tokens;
        string clean = cleanCodes(mshbits[i]);
        split(tokens, clean, is_any_of(" "));
        if (tokens.size() == 0)
          break;
        TetrinetMessage hash = (TetrinetMessage)djb2(tokens.front().c_str());
        tokens.pop_front();
        this->ProcessCommand(hash, &tokens);
      }
    }
  }
  catch (boost::system::error_code& e)
  {
    LOG4CXX_FATAL(logger, "Fatal error in connect: " << e.message());
  }
}

inline string TetrinetClient::cleanCodes(string orig)
{
  vector<char> newV(orig.length()), oldV(orig.begin(), orig.end());
  remove_copy_if(oldV.begin(), oldV.end(), newV.begin(), [](char c) { return iscntrl(c); });
  return string(newV.begin(), newV.end());
}

void TetrinetClient::SendCommand(TetrinetMessage message, std::string param)
{
  stringstream str;
  str << MessageMap.at(message) << " ";
  switch(message)
  {
  case TetrinetMessage::F:
    str << playernum << " " << param << " ";
    break;
  default:
    str << param;
    break;
  }
  str << "\xFF";
  string sstr = str.str();
  write(socket, buffer(string(sstr.begin(), sstr.end()), 1024));
}

void TetrinetClient::ProcessCommand(TetrinetMessage message, std::deque<std::string>* tokens)
{
  switch(message)
  {
    case TetrinetMessage::PLAYERNUM:
      plyrids.insert(pair<int, string>(playernum = atoi(tokens->at(0).substr(0, 1).c_str()), screenName));
      break;
    case TetrinetMessage::PLAYERJOIN:
      plyrids.insert(pair<int, string>(atoi(tokens->at(0).substr(0, 1).c_str()), tokens->at(1)));
      break;
    case TetrinetMessage::PLAYERLEAVE:
      plyrids.erase(atoi(tokens->at(0).substr(0, 1).c_str()));
      break;
    case TetrinetMessage::NOCONNECTING:
      LOG4CXX_ERROR(logger, "Noconnect received: " << tokens->at(0));
      break;
  }
}

const std::string* TetrinetClient::GetName() const
{
  return &screenName;
}

const int* TetrinetClient::GetID() const
{
  return &playernum;
}

void TetrinetClient::JoinChannel(const std::string channel)
{
  this->SendCommand(TetrinetMessage::PLINE, "/join " + channel);
}
