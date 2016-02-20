#ifndef SPECIALPIECES
#define SPECIALPIECES   \
  X(ADDLINE, a)		\
  X(CLEARLINE, c)	\
  X(NUKE, n)		\
  X(RANDOMCLEAR, r)	\
  X(SWITCH, s)		\
  X(CLEARSPECIALS, b)	\
  X(GRAVITY, g)		\
  X(QUAKE, q)		\
  X(BOMB, o)

#include <set>
#include "CompilerMagic.hpp"
#include "TetrinetForward.hpp"

#define X(a, b) a=TOCHAR(b),
/**
 * Represents all special pieces.  Subset of FieldElement
 */
enum class SpecialPiece : char { SPECIALPIECES };
#undef X

#define X(a, b) SpecialPiece::a,
/**
 * Convenience object.  Used for iterating over all pieces in enum
 */
const std::set<SpecialPiece> AllSpecialPiece = { SPECIALPIECES };
#undef X

bool isSpecial(FieldElement e);

#endif
