#include "libtetrinet/SpecialPiece.hpp"

#include "CompilerMagic.hpp"

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
AllSpecialPiece = { SPECIALPIECES };
#undef X
