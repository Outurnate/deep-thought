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
#include "libtetrinet/PieceLocation.hpp"
#include "libtetrinet/TetrinetConnection.hpp"
#include "libtetrinet/GameSettings.hpp"

struct TetrinetPlayer
{
  Field field;
  std::string name, team;
  unsigned level;
  bool playing;

  TetrinetPlayer(std::string name, std::string team) : field(), name(name), team(team), level(0), playing(false) {}
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
  const boost::signals2::connection AddOnPlayerWin  (const GenericSignal::slot_type& slot) const;
  const boost::signals2::connection AddOnPlayerLost (const GenericSignal::slot_type& slot) const;
protected:
  virtual PieceLocation NewPiece(const Piece& piece) = 0;
  const Field& GetField() const;
private:
  /** Processes a command issued from the server */
  void processCommand(TetrinetMessage message, std::deque<std::string>& tokens);
  /** Send the given message with supplied params to the sever */
  void sendCommand(TetrinetMessage message, std::string param);
  /** Magic function for encoding start message */
  inline std::string encode(std::string name, std::string address);
  void setField(unsigned num, const std::string& name);
  void placer();
  
  std::shared_ptr<boost::asio::io_service> service;
  std::unique_ptr<boost::asio::ip::tcp::socket> socket;
  std::map<unsigned, std::unique_ptr<TetrinetPlayer> > players;
  log4cxx::LoggerPtr logger;
  boost::optional<unsigned> playerNum;
  boost::optional<GameSettings> gameData;
  bool connected, inGame, paused;
  
  mutable GenericSignal onField;
  mutable GenericSignal onSpecial;
  mutable GenericSignal onMessage;
  mutable GenericSignal onPause;
  mutable GenericSignal onResume;
  mutable GenericSignal onGameStart;
  mutable GenericSignal onGameEnd;
  mutable GenericSignal onConnect;
  mutable GenericSignal onDisconnect;
  mutable GenericSignal onPlayerJoin;
  mutable GenericSignal onPlayerLeave;
  mutable GenericSignal onPlayerWin;
  mutable GenericSignal onPlayerLost;

  /** AI screen name */
  const std::string screenName;
};

#endif
