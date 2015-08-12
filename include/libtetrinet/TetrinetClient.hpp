#ifndef TETRINETCLIENT_H
#define TETRINETCLIENT_H

#include <map>
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/functional/hash.hpp>
#include <boost/signals2.hpp>
#include <boost/signals2/connection.hpp>
#include <log4cxx/logger.h>

#include "libtetrinet/Field.hpp"
#include "libtetrinet/TetrinetConnection.hpp"

struct TetrinetPlayer
{
  Field field;
  std::string name, team;
  unsigned level;
  bool playing;

  TetrinetPlayer(std::string name, std::string team) : field(), name(name), team(team), level(0), playing(false) {}
};

struct GameSettings
{
  unsigned startHeight, startLevel, lineLevel, levelIncr, lineSpecial, specialCount, specialCap, seed;
  std::map<double, PieceShape> blockFrequency;
  std::map<double, SpecialPiece> specialFrequency;
  bool showAvgLevel, classic;

  GameSettings(unsigned startHeight, unsigned startLevel, unsigned lineLevel, unsigned levelIncr,
	       unsigned lineSpecial, unsigned specialCount, unsigned specialCap, std::string blockFrequency,
	       std::string specialFrequency, bool showAvgLevel, bool classic, unsigned seed)
    : startHeight(startHeight), startLevel(startLevel), lineLevel(lineLevel), levelIncr(levelIncr),
      lineSpecial(lineSpecial), specialCount(specialCount), specialCap(specialCap), seed(seed),
      blockFrequency(), specialFrequency(), showAvgLevel(showAvgLevel), classic(classic)
  {
    double blockSize = blockFrequency.size(),
      specSize = specialFrequency.size(),
      blockSum = 0, specSum = 0;

    for (PieceShape shape : AllPieceShape)
      this->blockFrequency.emplace(blockSum += (double(std::count(blockFrequency.begin(), blockFrequency.end(), char(shape))) / blockSize), shape);
    
    for (SpecialPiece special : AllSpecialPiece)
      this->specialFrequency.emplace(specSum += (double(std::count(specialFrequency.begin(), specialFrequency.end(), char(special))) / specSize), special);
  }
};

class TetrinetClient : private boost::noncopyable
{
public:
  typedef boost::signals2::signal<void (const TetrinetClient&)> GenericSignal;
  
  TetrinetClient(std::string nickname, log4cxx::LoggerPtr logger);
  virtual ~TetrinetClient();
  
  /**
   * Connect to the given server
   *
   * Manages connection to the server, parses packets and dispenses them to ProcessCommand
   */
  virtual void Run(std::shared_ptr<boost::asio::io_service> service, TetrinetConnection& connection);
  /**
   * Performs a graceful stop
   */
  virtual void Stop();
  /**
   * Gets AI name
   */
  const std::string GetName() const;
  /**
   * Gets player ID
   */
  int GetID() const;
  const boost::signals2::connection AddOnComplete   (const GenericSignal::slot_type& slot) const;
  const boost::signals2::connection AddOnField      (const GenericSignal::slot_type& slot) const;
  const boost::signals2::connection AddOnSpecial    (const GenericSignal::slot_type& slot) const;
  const boost::signals2::connection AddOnMessage    (const GenericSignal::slot_type& slot) const;
  const boost::signals2::connection AddOnPause      (const GenericSignal::slot_type& slot) const;
  const boost::signals2::connection AddOnResume     (const GenericSignal::slot_type& slot) const;
  const boost::signals2::connection AddOnGameStart  (const GenericSignal::slot_type& slot) const;
  const boost::signals2::connection AddOnGameEnd    (const GenericSignal::slot_type& slot) const;
  const boost::signals2::connection AddOnConnect    (const GenericSignal::slot_type& slot) const;
  const boost::signals2::connection AddOnDisconnect (const GenericSignal::slot_type& slot) const;
  const boost::signals2::connection AddOnPlayerJoin (const GenericSignal::slot_type& slot) const;
  const boost::signals2::connection AddOnPlayerLeave(const GenericSignal::slot_type& slot) const;
  const boost::signals2::connection AddOnPieceReady (const GenericSignal::slot_type& slot) const;
protected:
  /**
   * Send the given message with supplied params to the sever
   */
  void SendCommand(TetrinetMessage message, std::string param);
  /**
   * Processes a command issued from the server
   */
  virtual void ProcessCommand(TetrinetMessage message, std::deque<std::string>& tokens);
private:
  /** Converts int to hex string */
  inline std::string makeHex(int dec);
  /** Magic function for encoding start message */
  inline std::string encode(std::string name, std::string address);
  /** Removes control codes from a string */
  inline std::string cleanCodes(std::string orig);
  void setField(unsigned num, const std::string& name);
  
  std::shared_ptr<boost::asio::io_service> service;
  std::unique_ptr<boost::asio::ip::tcp::socket> socket;
  std::map<unsigned, std::unique_ptr<TetrinetPlayer> > players;
  std::map<unsigned, std::string> playerNames;
  log4cxx::LoggerPtr logger;
  boost::optional<unsigned> playerNum;
  boost::optional<GameSettings> gameData;
  bool connected, inGame, paused;
  
  mutable GenericSignal onField;//
  mutable GenericSignal onSpecial;
  mutable GenericSignal onMessage;
  mutable GenericSignal onPause;//
  mutable GenericSignal onResume;//
  mutable GenericSignal onGameStart;//
  mutable GenericSignal onGameEnd;//
  mutable GenericSignal onConnect;//
  mutable GenericSignal onDisconnect;//
  mutable GenericSignal onPlayerJoin;//
  mutable GenericSignal onPlayerLeave;//
  mutable GenericSignal onPieceReady;

  /** AI screen name */
  const std::string screenName;
};

#endif
