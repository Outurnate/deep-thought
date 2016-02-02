#ifndef SOLVERBASE_HPP
#define SOLVERBASE_HPP

#include "DeepThoughtForward.hpp"

#include "MatchResult.hpp"

#include <boost/optional.hpp>
#include <boost/thread/thread.hpp>
#include <queue>

class SolverBase : private boost::noncopyable
{
public:
  SolverBase(std::vector<MatchPtr> matches);
  void Run(); // run threadpool and joinall
protected:
  virtual MatchResult RunMatch(const GenomePtr& a, const GenomePtr& b, unsigned matchId) = 0;
private:
  void RunMatchDispatch(const MatchPtr& match, unsigned matchId);

  std::vector<MatchPtr> matches;
  std::queue<MatchResult> resultQueue;
  
  boost::condition_variable condition;
  boost::mutex mute;
  bool dataReady;
};

#endif
