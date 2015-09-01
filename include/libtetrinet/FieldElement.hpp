#ifndef FIELDELEMENT_HPP
#define FIELDELEMENT_HPP

#include "libtetrinet/TetrinetForward.hpp"

/**
 * Valid values for cells in a field
 */
enum class FieldElement : char
{
  BLUE          = '1',
  YELLOW        = '2',
  GREEN         = '3',
  PURPLE        = '4',
  RED           = '5',

  NONE          = '0',

  ADDLINE       = 'a',
  CLEARLINE     = 'c',
  NUKE          = 'n',
  RANDOMCLEAR   = 'r',
  SWITCH        = 's',
  CLEARSPECIALS = 'b',
  GRAVITY       = 'g',
  QUAKE         = 'q',
  BOMB          = 'o',

  UNDEFINED     = '-' // used to specify ignorance in transform
};

/**
 * Retrieves a random color valid for the given piece
 */
FieldElement GetColor(const PieceShape& shape);

/**
 * FieldElement stream operator
 */
std::ostream& operator<<(std::ostream& os, const FieldElement& element);

#endif
