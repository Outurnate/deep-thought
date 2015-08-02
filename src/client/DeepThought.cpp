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
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <fstream>
#include <signal.h>
#include <string>
#include <iostream>

#include "libdeepthought/AIManager.hpp"
#include "libsimplemarkov/MarkovGenerator.hpp"

using namespace log4cxx;
using namespace std;
using namespace boost;
using namespace boost::archive;
using namespace boost::program_options;

AIManager* manager;
condition killCondition;
mutex killMutex;
bool sigself = false;

void winchHandler(int param)
{
//  interface->Resize();
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

int main(int argc, char *argv[])
{
  mutex::scoped_lock lock(killMutex);

  options_description desc("Usage");

  desc.add_options()
    ("help,h", "Display this message")
    ("version,v", "Version info")
    ("server,s", value<string>()->required(), "Hostname or address of server to connect to")
    ("logfile,l", value<string>()->default_value("deep-thought.log"), "Log file path")
    ("corpus,c", value<string>()->required()->default_value("corpus.txt"), "Corpus file path")
    ("generate-corpus,g", "Generate a corpus from stdin");

  variables_map opt;
  store(command_line_parser(argc, argv).options(desc).run(), opt);

  if (opt.count("help"))
  {
    cout << "deep-thought  Copyright (C) 2015  Joseph D" << endl
         << "This program comes with ABSOLUTELY NO WARRANTY; distributed under the GNU GPLv3" << endl
         << "This is free software, and you are welcome to redistribute it." << endl
         << "http://www.gnu.org/licenses/gpl-3.0.html" << endl << endl
         << desc;
    return 0;
  }

  try
  {
    notify(opt);
  }
  catch (required_option& e)
  {
    cerr << "Invalid command line: " << e.what() << endl
	 << "Use -h or --help for options" << endl;
    return 1;
  }
  
  // Create loggers
  FileAppender* fileAppender = new FileAppender(LayoutPtr(new SimpleLayout()), opt["logfile"].as<string>(), false);
//  interface = new TermInterface();
//  interface->setLayout(LayoutPtr(new SimpleLayout()));

  helpers::Pool p;
  fileAppender->activateOptions(p);

  BasicConfigurator::configure(AppenderPtr(fileAppender));
//  BasicConfigurator::configure(AppenderPtr(interface));
  Logger::getRootLogger()->setLevel(Level::getTrace());

  // Register signal handlers
  struct sigaction winchSA, sigintSA;
  winchSA.sa_handler = winchHandler;
  sigintSA.sa_handler = sigintHandler;
  sigemptyset(&winchSA.sa_mask);
  sigemptyset(&sigintSA.sa_mask);
  winchSA.sa_flags = SA_RESTART;
  sigintSA.sa_flags = SA_RESTART;
  if (sigaction(SIGWINCH, &winchSA, NULL) == -1) exit(1);
  if (sigaction(SIGINT, &sigintSA, NULL) == -1) exit(1);

  manager = new AIManager();
  //manager->RegisterStatusHandler(interface);

  killCondition.wait(lock); // release lock and wait for kill

  delete manager;
  //delete interface;
  
  if (sigself) // if we got sigint, pass on the sig
  {
    signal(SIGINT, SIG_DFL);
    kill(getpid(), SIGINT);
  }
  
  return 0;
}
