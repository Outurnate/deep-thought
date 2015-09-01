#ifndef PIECESHAPES
#define PIECESHAPES \
  X(I)		    \
  X(O)		    \
  X(J)		    \
  X(L)		    \
  X(S)		    \
  X(Z)		    \
  X(T)

#include <vector>

#include "CompilerMagic.hpp"

#define X(a) a=TOCHAR(a),
/**
 * Represents tetramino shapes
 */
enum class PieceShape : char { PIECESHAPES };
#undef X

/**
 * Convenience object for iterating over all piece shapes
 */
extern const std::vector<PieceShape> AllPieceShape;

#endif
