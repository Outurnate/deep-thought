#include "libtetrinet/TetrinetMessage.hpp"

#include <algorithm>

using namespace std;

const string stringToLower(const string& str)
{
  string ret(str);
  transform(str.begin(), str.end(), ret.begin(), ::tolower);
  return ret;
}

const string GetMessageString(const TetrinetMessage& message)
{
# define X(a) case TetrinetMessage::a:return stringToLower(string(#a));
  switch(message)
  {
    TETRINETMESSAGES
  }
# undef X
}
