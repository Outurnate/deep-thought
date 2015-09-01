#ifndef PIECEROTATION_HPP
#define PIECEROTATION_HPP

#include <stdint.h>

/**
 * Represents four rotation states (zero, right, twice, left)
 */
enum class PieceRotation : uint8_t
{
  Z = 0,
  R = 1,
  T = 2,
  L = 3
};

#endif
