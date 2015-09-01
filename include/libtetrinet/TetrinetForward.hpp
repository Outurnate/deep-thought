#ifndef TETRINETFORWARD_HPP
#define TETRINETFORWARD_HPP

#include <stdint.h>
#include <vector>
#include <map>
#include <array>
#include <iosfwd>

enum class FieldElement : char;
enum class PieceRotation : uint8_t;
enum class PieceShape : char;
enum class ProtocolVersion : int;
enum class RotationDirection : bool;
enum class SpecialPiece : char;
enum class TetrinetMessage : unsigned long;

class FieldEvaluator;
class Field;
class FieldTransform;
class GameSettings;
class Piece;
class PieceLocation;
class TetrinetClient;
class TetrinetConnection;

typedef uint16_t uCoord;
typedef  int32_t sCoord;
typedef uint32_t uAxis;
typedef std::pair<PieceRotation, PieceRotation> PieceRotationPair;
typedef std::pair<sCoord, sCoord> TransformPair;
typedef std::vector<FieldElement> FieldType; // origin top left, row-major
typedef std::map<PieceRotationPair, std::array<TransformPair, 5> > SRSKickMap;

static const uAxis fieldWidth = 12;
static const uAxis fieldHeight = 22;
static const uAxis fieldSize = fieldWidth * fieldHeight;

#endif
