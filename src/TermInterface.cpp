#include "TermInterface.hpp"

#include <string>

#include "Constants.hpp"

using namespace std;
using namespace boost;
using namespace log4cxx;
using namespace log4cxx::spi;
using namespace log4cxx::helpers;

IMPLEMENT_LOG4CXX_OBJECT(TermInterface)

TermInterface::TermInterface() : logger(Logger::getLogger("ui"))
{
  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  start_color();
  use_default_colors();

  init_pair(1, COLOR_GREEN, COLOR_BLACK);

  boardWindow   = newwin(0, 0, 0, 0);
  barWindow     = newwin(0, 0, 0, 0);
  statusWindow  = newwin(0, 0, 0, 0);
  messageWindow = newwin(0, 0, 0, 0);
  
  Resize();
}

TermInterface::~TermInterface()
{
  destroyWin(boardWindow);
  destroyWin(barWindow);
  destroyWin(statusWindow);
  destroyWin(messageWindow);
  endwin();
}

void TermInterface::Resize()
{
  endwin();
  refresh();
  clear();
  
  wclear(boardWindow);
  wclear(barWindow);
  wclear(statusWindow);
  wclear(messageWindow);
  
  wresize(boardWindow, FIELD_HEIGHT + 2, FIELD_WIDTH + 2);
  wresize(barWindow, 3, COLS - 4);
  wresize(statusWindow, FIELD_HEIGHT + 2, COLS - FIELD_WIDTH - 8);
  wresize(messageWindow, LINES - FIELD_HEIGHT - 9, COLS - 4);
  
  mvwin(boardWindow, 5, 1);
  mvwin(barWindow, 1, 1);
  mvwin(statusWindow, 5, FIELD_WIDTH + 5);
  mvwin(messageWindow, FIELD_HEIGHT + 8, 1);

  borderWin(boardWindow);
  borderWin(barWindow);
  borderWin(statusWindow);
  borderWin(messageWindow);
  
  wrefresh(boardWindow);
  wrefresh(barWindow);
  wrefresh(statusWindow);
  wrefresh(messageWindow);

  LOG4CXX_TRACE(logger, "TEST TEST TEST");
  
  messageLength = LINES - FIELD_HEIGHT - 11; // size_of_message_buffer - 2
  shrinkMessages();
}

void TermInterface::shrinkMessages()
{
  if (messageLength < messageList.size())
  {
    list<string>::iterator end = messageList.begin();
    advance(end, messageList.size() - messageLength);
    messageList.erase(messageList.begin(), end);
  }
  messageList.resize(messageLength);
  wclear(messageWindow);
  borderWin(messageWindow);
  unsigned row = 1, maxWidth = COLS - 6; // window width - 2
  for (string& msg : messageList)
    mvwprintw(messageWindow, row++, 1, msg.length() > maxWidth ? msg.substr(0, maxWidth).c_str() : msg.c_str());
  wrefresh(messageWindow);
}

void TermInterface::borderWin(WINDOW* win)
{
  wattron(win, COLOR_PAIR(PAIR_BORDER));
  wborder(win, '|', '|', '-','-','+','+','+','+');
  wattroff(win, COLOR_PAIR(PAIR_BORDER));
}

void TermInterface::Show()
{
  while (2>1) //just spin...blurgh
  {
  }
}

void TermInterface::HandleStatus(AIStatus status)
{
  string field(status.field.begin(), status.field.end());
  for (unsigned row = 0; row < FIELD_HEIGHT; ++row)
    mvwprintw(boardWindow, row + 1, 1, field.substr(row * FIELD_WIDTH, FIELD_WIDTH).c_str());
  wrefresh(boardWindow);
}

void TermInterface::destroyWin(WINDOW* win)
{
  wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
  wrefresh(win);
  delwin(win);
}

void TermInterface::append(const LoggingEventPtr& event, Pool& p)
{
  if (this->layout == NULL)
  {
    LOG4CXX_ENCODE_CHAR(nameStr, name);
    string msg("No Layout set for the appender named [ ");
    msg.append(nameStr);
    msg.append(" ].");

    LOG4CXX_DECODE_CHAR(msgL, msg);
    errorHandler->error(msgL);
    return;
  }

  LogString msg;
  this->layout->format(msg, event, p);
  LOG4CXX_ENCODE_CHAR(msgStr, msg);
  messageList.push_back(msgStr);
  if (messageList.size() == messageLength)
    messageList.pop_front();
  shrinkMessages();
}

void TermInterface::close()
{
  this->closed = true;
}
