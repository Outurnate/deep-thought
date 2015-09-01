#include "libtetrinet/PieceShape.hpp"

#define X(a) PieceShape::a,
const std::vector<PieceShape> AllPieceShape = { PIECESHAPES };
#undef X
