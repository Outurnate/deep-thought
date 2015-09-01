#include "libtetrinet/TetrinetMessage.hpp"

using namespace std;

const string GetMessageString(const TetrinetMessage& message)
{
# define X(a) case TetrinetMessage::a:return stringToLower(string(#a));
  switch(message)
  {
    TETRINETMESSAGES
  }
# undef X
}
