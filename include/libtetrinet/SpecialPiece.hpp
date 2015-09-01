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

#include <vector>

/**
 * Represents all special pieces.  Subset of FieldElement
 */
enum class SpecialPiece : char;

/**
 * Convenience object.  Used for iterating over all pieces in enum
 */
extern const std::vector<SpecialPiece> AllSpecialPiece;

#endif
