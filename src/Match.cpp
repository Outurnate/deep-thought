#include "Match.hpp"

Match::Match(AIEngine* playerOne, AIEngine* playerTwo) : playerOne(playerOne), playerTwo(playerTwo)
{
}

Match::~Match()
{
}

bool Match::CanPlayNow()
{
  return playerOne->GetState() == AIState::IDLE && playerTwo->GetState() == AIState::IDLE;
}

void Match::QueueMatch()
{
  playerOne->SetWait(true);
  playerTwo->SetWait(true);
}

void Match::Start(std::string channel)
{
  playerOne->JoinChannel(channel);
  playerTwo->JoinChannel(channel);
}
