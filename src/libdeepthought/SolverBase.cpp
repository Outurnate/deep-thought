#include "libdeepthought/SolverBase.hpp"

#include <boost/thread/mutex.hpp>

#include "libdeepthought/MatchPtr.hpp"

using namespace std;
using namespace boost;

SolverBase::SolverBase(vector<MatchPtr> matches)
  : matches(matches)
{
}

void SolverBase::Run()
{
  for (unsigned i = 0; i < 100; ++i)
    threads.add_thread(new thread(&SolverBase::operator(), this));
  threads.join_all();
}

optional<MatchPtr> SolverBase::removeFromStack()
{
  unique_lock<mutex> lock(mute);
  if (matches.empty())
    return none;
  else
  {
    auto m(matches.top());
    matches.pop();
    return m;
  }
}

void SolverBase::operator() ()
{
  while (true)
  {
    auto m = removeFromStack();
    if (m)
    {
      try
      {
        RunMatch(m.get());
      }
      catch (Wt::Dbo::Exception& e)
      {
        std::cout << e.what() << std::endl;
      }
    }
    else
      break;
  }
}
