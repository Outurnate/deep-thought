#include "libdeepthought/SolverBase.hpp"

#include <boost/thread/mutex.hpp>
#include <boost/asio/io_service.hpp>

#include "libdeepthought/Genome.hpp"
#include "libdeepthought/Match.hpp"
#include "libdeepthought/MatchPtr.hpp"

using namespace std;
using namespace boost;
using namespace boost::asio;

SolverBase::SolverBase(vector<MatchPtr> matches)
  : matches(matches), dataReady(false)
{
}

void SolverBase::Run()
{
  thread_group threads;
  io_service service;
  
  unsigned matchId = 0;
  for (const MatchPtr& match : matches)
    service.post(bind(&SolverBase::RunMatchDispatch, this, match, matchId++));
  
  for (unsigned i = 0; i < 100; ++i)
    threads.create_thread([&]()
                          {
                            service.run();
                          });
  bool isPoolClosed = false;
  thread poolMonitor([&]()
                     {
                       threads.join_all();
                       dataReady = true;
                       isPoolClosed = true;
                       condition.notify_all();
                     });

//  while (!all_of(matches.begin(), matches.end(), [](const MatchPtr& match)
//                 {
//                   return match->complete;
//                 }))
  while(!isPoolClosed)
  {
    unique_lock<mutex> lock(mute);
    while(!dataReady)
      condition.wait(lock);
    while(!resultQueue.empty())
    {
      auto res = resultQueue.front();
      resultQueue.pop();
      res.winnerIsGenomeA = true;
      matches[res.matchId].SetResult(res);
    }
    dataReady = false;
  }
  
  threads.join_all();
}

void SolverBase::RunMatchDispatch(const MatchPtr& match, unsigned matchId)
{
  auto res = RunMatch(*match->genomeA, *match->genomeB, matchId);
  {
    lock_guard<mutex> lock(mute);
    dataReady = true;
    resultQueue.push(res);
  }
  condition.notify_one();
}
