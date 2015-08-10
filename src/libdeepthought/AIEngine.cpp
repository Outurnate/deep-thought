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
      setState(AIState::RUNNING);
      placer = new thread(&AIEngine::DoPlacing, this);
    }
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
