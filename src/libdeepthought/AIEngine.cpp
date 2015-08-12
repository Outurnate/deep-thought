/*
 * AIEngine.cpp
 *
 *  Created on: Oct 26, 2012
 *      Author: joseph
 */

#include <cctype>

#include "libdeepthought/AIEngine.hpp"

#include "Constants.hpp"

using boost::asio::ip::tcp;

using namespace std;
using namespace log4cxx;
using namespace boost;

AIEngine::AIEngine(Genome genome, LoggerPtr logger)
  : TetrinetClient(genome.Name, logger), pieceDelay(1), logger(logger), placer(0), currentState(AIState::IDLE)
{
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
    fmtx.lock();
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

void AIEngine::ProcessCommand(TetrinetMessage message, std::deque<std::string>& tokens)
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
        for (long unsigned i = 1; i < tokens.size(); ++i)
          msg << tokens.at(i) << " ";
        LOG4CXX_INFO(logger, "MSG: " << msg.str());
      }
      break;

    case TetrinetMessage::NEWGAME:
      setState(AIState::RUNNING);
      placer = new thread(&AIEngine::DoPlacing, this);
      break;

    case TetrinetMessage::ENDGAME:
      setState(AIState::IDLE);
      Stop();
      break;
    }
  }
  catch (out_of_range& e)
  {
  }
}

AIState AIEngine::GetState() const
{
  return currentState;
}

void AIEngine::SetWait(bool waiting)
{
  if (currentState != AIState::RUNNING)
    setState(waiting ? AIState::WAITING : AIState::IDLE);
}

void AIEngine::setState(AIState state)
{
  currentState = state;
}
