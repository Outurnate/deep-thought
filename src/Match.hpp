#ifndef MATCH_HPP
#define MATCH_HPP

#include <string>

class Match
{
public:
  //Match(AIEngine* playerOne, AIEngine* playerTwo);
  virtual ~Match();

  bool CanPlayNow(); // check if both players are free
  void QueueMatch(); // sets both bots to 'queued' so another match cannot use them
  void Start(std::string channel);
private:
  //AIEngine *playerOne, *playerTwo;
};

#endif
