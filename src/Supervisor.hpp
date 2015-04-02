#ifndef SUPERVISOR_HPP
#define SUPERVISOR_HPP

#include "TetrinetClient.hpp"
#include "AIEngine.hpp"
#include "AIManager.hpp"

/**
 * Supervisor manages rooms, issues commands, and generally wrangles bots
 * Acts as a frontend to AIManager
 */
class Supervisor : public TetrinetClient
{
public:
  Supervisor(AIManager* manager);
  virtual ~Supervisor();
protected:
  void ProcessCommand(TetrinetMessage message, std::deque<std::string>* tokens) override;
private:
  AIManager* manager;
};

#endif
