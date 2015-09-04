#ifndef TETRINETMESSAGES
#define TETRINETMESSAGES \
  X(F)			 \
  X(SB)			 \
  X(LVL)		 \
  X(GMSG)		 \
  X(TEAM)		 \
  X(PLINE)		 \
  X(PAUSE)		 \
  X(INGAME)		 \
  X(NEWGAME)		 \
  X(ENDGAME)		 \
  X(WINLIST)		 \
  X(PLINEACT)		 \
  X(PLAYERWON)		 \
  X(PLAYERNUM)		 \
  X(PLAYERLOST)		 \
  X(PLAYERJOIN)		 \
  X(PLAYERLEAVE)	 \
  X(NOCONNECTING)

#include <string>

#include "CompilerMagic.hpp"

#define X(a) a=djb2(#a),
/**
 * Defines all messages in the tetrinet v1.13/v1.14 protocol
 */
enum class TetrinetMessage : unsigned long { TETRINETMESSAGES };
#undef X

/**
 * Converts enum value hash back to text, for sending
 */
const std::string GetMessageString(const TetrinetMessage& message);

const std::string stringToUpper(const std::string& str);
const std::string stringToLower(const std::string& str);

#endif
