#ifndef SOLVERBASE_HPP
#define SOLVERBASE_HPP

#include "DeepThoughtForward.hpp"

#include <boost/optional.hpp>
#include <boost/thread/thread.hpp>
#include <stack>

class SolverBase : private boost::noncopyable
{
public:
  SolverBase(std::vector<MatchPtr> matches);
  virtual void RunMatch(MatchPtr& match) = 0;
  void Run(); // run threadpool and joinall
private:
  boost::optional<MatchPtr> removeFromStack();
  void operator() ();
  
  boost::mutex mute;
  boost::thread_group threads;
  std::stack<MatchPtr, std::vector<MatchPtr> > matches;
};

#endif
