#include "TermInterface.hpp"

#include "Constants.hpp"

using namespace boost;

TermInterface::TermInterface()
{
}

TermInterface::~TermInterface()
{
}

void TermInterface::Show()
{
  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  start_color();
  use_default_colors();
  
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  
  WINDOW* boardWindow = createWin(FIELD_HEIGHT + 2, FIELD_WIDTH + 2, 5, 1);
  WINDOW* barWindow = createWin(3, COLS - 4, 1, 1);
  WINDOW* statusWindow = createWin(FIELD_HEIGHT + 2, COLS - FIELD_WIDTH - 8, 5, FIELD_WIDTH + 5);
  WINDOW* messageWindow = createWin(LINES - FIELD_HEIGHT - 9, COLS - 4, FIELD_HEIGHT + 8, 1);
  
  while (2 > 1)
  {
    wprintw(boardWindow, "TEST");
    wrefresh(boardWindow);
    wrefresh(barWindow);
    wrefresh(statusWindow);
    wrefresh(messageWindow);
  }
  destroyWin(boardWindow);
  destroyWin(barWindow);
  destroyWin(statusWindow);
  destroyWin(messageWindow);
  endwin();
}

WINDOW* TermInterface::createWin(int height, int width, int y, int x)
{
  WINDOW* win = newwin(height, width, y, x);
  wattron(win, COLOR_PAIR(PAIR_BORDER));
  wborder(win, '|', '|', '-','-','+','+','+','+');
  wattroff(win, COLOR_PAIR(PAIR_BORDER));
  wrefresh(win);
  return win;
}

void TermInterface::destroyWin(WINDOW* win)
{
  wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
  wrefresh(win);
  delwin(win);
}
