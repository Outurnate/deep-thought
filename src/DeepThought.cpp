/*
 * DeepThought.cpp
 *
 *  Created on: Oct 26, 2012
 *      Author: joseph
 */

#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <signal.h>

#include "TermInterface.hpp"
#include "AIManager.hpp"

using namespace log4cxx;
using namespace log4cxx::xml;

AIManager* manager;
TermInterface* interface;

void winchHandler(int param)
{
  interface->Resize();
}

int main()
{
  signal(SIGWINCH, winchHandler);
  DOMConfigurator::configure("Log4cxxConfig.xml");
  
  manager = new AIManager();
  interface = new TermInterface();
  manager->RegisterStatusHandler(interface);

  interface->Show();
  manager->Start();

  delete manager;
  delete interface;
  return 0;
}
