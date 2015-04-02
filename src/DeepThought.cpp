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
#include <signal.h>

#include "TermInterface.hpp"
#include "AIManager.hpp"

using namespace log4cxx;

AIManager* manager;
TermInterface* interface;

void winchHandler(int param)
{
  interface->Resize();
}

int main()
{
  signal(SIGWINCH, winchHandler);
  FileAppender* fileAppender = new FileAppender(LayoutPtr(new SimpleLayout()), "logfile", false);
  interface = new TermInterface();
  interface->setLayout(LayoutPtr(new SimpleLayout()));

  helpers::Pool p;
  fileAppender->activateOptions(p);

  BasicConfigurator::configure(AppenderPtr(fileAppender));
  BasicConfigurator::configure(AppenderPtr(interface));
  Logger::getRootLogger()->setLevel(Level::getTrace());

  manager = new AIManager();
  manager->RegisterStatusHandler(interface);

  while(true); //wheee

  delete manager;
  delete interface;
  return 0;
}
