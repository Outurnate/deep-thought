#include "TermInterface.hpp"

#include "Constants.hpp"

TermInterface::TermInterface() : engines()
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
  WINDOW* boardWindow = createWin(FIELD_HEIGHT + 2, FIELD_WIDTH + 2, 5, 1);
  WINDOW* barWindow = createWin(3, COLS - 4, 1, 1);
  while (2 > 1)
  {
    wrefresh(boardWindow);
    wrefresh(barWindow);
  }
  destroyWin(boardWindow);
  destroyWin(barWindow);
  endwin();
}

WINDOW* TermInterface::createWin(int height, int width, int y, int x)
{
  WINDOW* win = newwin(height, width, y, x);
//  box(win, 0, 0);
  wborder(win, '|', '|', '-','-','+','+','+','+');
  wrefresh(win);
  return win;
}

void TermInterface::destroyWin(WINDOW* win)
{
  wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
  wrefresh(win);
  delwin(win);
}
