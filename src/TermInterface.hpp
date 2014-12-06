#ifndef TERMINTERFACE_HPP
#define TERMINTERFACE_HPP

#include <ncurses.h>
#include <log4cxx/appenderskeleton.h>
#include <log4cxx/spi/loggingevent.h>
#include <log4cxx/helpers/object.h>
#include <string>
#include <list>

#include "AIEngine.hpp"
#include "AIManager.hpp"

namespace log4cxx
{
  class TermInterface : public IFieldStatusHandler, public AppenderSkeleton
  {
  public:
    DECLARE_LOG4CXX_OBJECT(TermInterface)

    BEGIN_LOG4CXX_CAST_MAP()
      LOG4CXX_CAST_ENTRY(TermInterface)
      LOG4CXX_CAST_ENTRY_CHAIN(AppenderSkeleton)
    END_LOG4CXX_CAST_MAP()
    
    TermInterface();
    virtual ~TermInterface();

    void Show();
    void Resize();

    void HandleStatus(AIStatus status);
    
    void append(const spi::LoggingEventPtr& event, log4cxx::helpers::Pool& p);
    void close();
    bool isClosed() const { return closed; }
    bool requiresLayout() const { return true; }
  private:
    void borderWin(WINDOW* win);
    void destroyWin(WINDOW* win);
    void shrinkMessages();

    WINDOW *boardWindow, *barWindow, *statusWindow, *messageWindow;
    std::list<std::string> messageList;
    unsigned messageLength;
    log4cxx::LoggerPtr logger;
  };
}

#endif
