/*
 * DeepThought.cpp
 *
 *  Created on: Oct 26, 2012
 *      Author: joseph
 */

#include <log4cxx/logger.h>
#include <log4cxx/helpers/pool.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/simplelayout.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/thread/condition.hpp>
#include <fstream>
#include <signal.h>

#include "TermInterface.hpp"
#include "AIManager.hpp"
#include "MarkovGenerator.hpp"

using namespace log4cxx;
using namespace std;
using namespace boost;
using namespace boost::archive;

AIManager* manager;
TermInterface* interface;
condition killCondition;
mutex killMutex;
bool sigself = false;

void winchHandler(int param)
{
  interface->Resize();
}

void sigintHandler(int param)
{
  mutex::scoped_lock lock(killMutex);
  killCondition.notify_one();
  sigself = true;
}

inline MarkovCorpus readCorpus()
{
  return MarkovCorpus::FromStream(cin);
}

inline MarkovCorpus loadCorpus(ifstream& in)
{
  MarkovCorpus n;
  text_iarchive(in) >> n;
  return n;
}

inline void saveCorpus(ofstream& out, const MarkovCorpus& n)
{
  text_oarchive(out) << n;
}

inline void quit()
{
  killCondition.notify_one();
}

int main()
{
  mutex::scoped_lock lock(killMutex);
  // Create loggers
  FileAppender* fileAppender = new FileAppender(LayoutPtr(new SimpleLayout()), "logfile", false);
  interface = new TermInterface();
  interface->setLayout(LayoutPtr(new SimpleLayout()));

  helpers::Pool p;
  fileAppender->activateOptions(p);

  BasicConfigurator::configure(AppenderPtr(fileAppender));
  BasicConfigurator::configure(AppenderPtr(interface));
  Logger::getRootLogger()->setLevel(Level::getTrace());

  // Register signal handlers
  struct sigaction winchSA;
  struct sigaction sigintSA;
  winchSA.sa_handler = winchHandler;
  sigintSA.sa_handler = sigintHandler;
  sigemptyset(&winchSA.sa_mask);
  sigemptyset(&sigintSA.sa_mask);
  winchSA.sa_flags = SA_RESTART;
  sigintSA.sa_flags = SA_RESTART;
  if (sigaction(SIGWINCH, &winchSA, NULL) == -1) exit(1);
  if (sigaction(SIGINT, &sigintSA, NULL) == -1) exit(1);

  manager = new AIManager();
  manager->RegisterStatusHandler(interface);

  killCondition.wait(lock); // release lock and wait for kill

  delete manager;
  delete interface;
  
  if (sigself) // if we got sigint, pass on the sig
  {
    signal(SIGINT, SIG_DFL);
    kill(getpid(), SIGINT);
  }
  
  return 0;
}
