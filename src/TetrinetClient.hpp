#ifndef TETRINETCLIENT_H
#define TETRINETCLIENT_H

#include <map>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/functional/hash.hpp>
#include <log4cxx/logger.h>

unsigned long constexpr djb2(const char* c);

unsigned long constexpr djb2(const char* c, unsigned long hash)
{
  return !*c ? hash : djb2(c + 1, ((hash << 5) + hash) + *c);
}

unsigned long constexpr djb2(const char* c)
{
  return djb2(c, 5381);
}

unsigned long constexpr
  TOKEN_F            = djb2("f"           ),
  TOKEN_SB           = djb2("sb"          ),
//TOKEN_LVL          = djb2("lvl"         ),
  TOKEN_GMSG         = djb2("gmsg"        ),
//TOKEN_TEAM         = djb2("team"        ),
  TOKEN_PLINE        = djb2("pline"       ),
  TOKEN_PAUSE        = djb2("pause"       ),
  TOKEN_NEWGAME      = djb2("newgame"     ),
  TOKEN_ENDGAME      = djb2("endgame"     ),
//TOKEN_WINLIST      = djb2("winlist"     ),
  TOKEN_PLINEACT     = djb2("plineact"    ),
//TOKEN_PLAYERWON    = djb2("playerwon"   ),
  TOKEN_PLAYERNUM    = djb2("playernum"   ),
//TOKEN_PLAYERLOST   = djb2("playerlost"  ),
  TOKEN_PLAYERJOIN   = djb2("playerjoin"  ),
  TOKEN_PLAYERLEAVE  = djb2("playerleave" ),
  TOKEN_NOCONNECTING = djb2("noconnecting");

enum class TetrinetMessage : unsigned long
{
  F            = TOKEN_F,
  SB           = TOKEN_SB,
//LVL          = TOKEN_LVL,
  GMSG         = TOKEN_GMSG,
//TEAM         = TOKEN_TEAM,
  PLINE        = TOKEN_PLINE,
  PAUSE        = TOKEN_PAUSE,
  NEWGAME      = TOKEN_NEWGAME,
  ENDGAME      = TOKEN_ENDGAME,
//WINLIST      = TOKEN_WINLIST,
  PLINEACT     = TOKEN_PLINEACT,
//PLAYERNUM    = TOKEN_PLAYERWON,
  PLAYERNUM    = TOKEN_PLAYERNUM,
//PLAYERLOST   = TOKEN_PLAYERLOST,
  PLAYERJOIN   = TOKEN_PLAYERJOIN,
  PLAYERLEAVE  = TOKEN_PLAYERLEAVE,
  NOCONNECTING = TOKEN_NOCONNECTING
};

const std::map<TetrinetMessage, std::string> MessageMap =
{
  { TetrinetMessage::F,            "f"            },
  { TetrinetMessage::SB,           "sb"           },
//{ TetrinetMessage::LVL,          "lvl",         },
  { TetrinetMessage::GMSG,         "gmsg"         },
//{ TetrinetMessage::TEAM,         "team"         },
  { TetrinetMessage::PLINE,        "pline"        },
  { TetrinetMessage::PAUSE,        "pause"        },
  { TetrinetMessage::NEWGAME,      "newgame"      },
  { TetrinetMessage::ENDGAME,      "endgame"      },
//{ TetrinetMessage::WINLIST,      "winlist"      },
  { TetrinetMessage::PLINEACT,     "plineact"     },
//{ TetrinetMessage::PLAYERNUM,    "playerwon"    },
  { TetrinetMessage::PLAYERNUM,    "playernum"    },
//{ TetrinetMessage::PLAYERLOST,   "playerlost"   },
  { TetrinetMessage::PLAYERJOIN,   "playerjoin"   },
  { TetrinetMessage::PLAYERLEAVE,  "playerleave"  },
  { TetrinetMessage::NOCONNECTING, "noconnecting" }
};
//GMSG,SB,PAUSE
class TetrinetClient
{
public:
  TetrinetClient(std::string nickname, log4cxx::LoggerPtr logger);
  TetrinetClient(const TetrinetClient& client);
  virtual ~TetrinetClient();
  
  /**
   * Connect to the given server
   *
   * Manages connection to the server, parses packets and dispenses them to ProcessCommand
   */
  virtual void Run();
  /**
   * Performs a graceful stop
   */
  virtual void Stop() {}
  /**
   * Gets AI name
   */
  const std::string* GetName() const;
  /**
   * Gets player ID
   */
  const int* GetID() const;
  /**
   * Executes /join for the given channel
   */
  void JoinChannel(const std::string channel);
protected:
  /**
   * Send the given message with supplied params to the sever
   */
  void SendCommand(TetrinetMessage message, std::string param);
  /**
   * Processes a command issued from the server
   */
  virtual void ProcessCommand(TetrinetMessage message, std::deque<std::string>* tokens);
private:
  
  /** Converts int to hex string */
  inline std::string makeHex(int dec);
  /** Magic function for encoding start message */
  inline std::string encode(std::string name, std::string address);
  /** Removes control codes from a string */
  inline std::string cleanCodes(std::string orig);
  
  boost::asio::io_service service;
  boost::asio::ip::tcp::socket socket;
  boost::asio::io_service& socio;
  log4cxx::LoggerPtr logger;
  std::map<int, std::string> plyrids;
  int playernum;

  /** AI screen name */
  const std::string screenName;
};

#endif
