#ifndef TETRINETFORWARD_HPP
#define TETRINETFORWARD_HPP

#include <stdint.h>
#include <vector>
#include <map>
#include <array>

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
typedef std::map<PieceShape, std::vector<Piece> > PieceDefinitionMap;

static const uAxis fieldWidth = 12;
static const uAxis fieldHeight = 22;
static const uAxis fieldSize = fieldWidth * fieldHeight;

static const uAxis pieceWidth = 4;
static const uAxis pieceHeight = 4;
static const uAxis pieceSize = pieceWidth * pieceHeight;

#endif
