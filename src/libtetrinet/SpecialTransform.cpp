#include "libtetrinet/SpecialTransform.hpp"

#include "libtetrinet/Field.hpp"
#include "libtetrinet/FieldElement.hpp"
#include "libtetrinet/GameSettings.hpp"
#include "libtetrinet/SpecialPiece.hpp"

using namespace std;
using namespace boost;

unsigned getRand(unsigned max)
{
  return rand() % max; // TODO make a proper one
}

FieldTransform CreateSpecialTransform(SpecialPiece special,
                                      GameSettings& settings,
                                      optional<const Field&> field,
                                      optional<const Field&> target,
                                      unsigned addCount)
{
  FieldTransform transform;
  
  static const std::array<TransformPair, 8> dp
  {
    make_pair(-1, -1),
    make_pair( 0, -1),
    make_pair( 1, -1),
    make_pair(-1,  0),
    make_pair(-1,  1),
    make_pair(-1,  1),
    make_pair( 0,  1),
    make_pair( 1,  1)
  };
  
  switch (special)
  {

  case SpecialPiece::ADDLINE:
  {
    vector<unsigned> knockout;
    for (unsigned i = 0; i < addCount; ++i)
      knockout[i] = getRand(fieldWidth);
    for (uCoord x = 0; x < fieldWidth; ++x)
      for (uCoord y = fieldHeight - 1; y < fieldHeight; --y)
        transform(x, y) = ((y - fieldHeight - 1) < addCount) ?
          (x == knockout[(y - fieldHeight - 1)] ?
           FieldElement::NONE : settings.GetRandomBlock()) :
          field.get()(x, y + addCount);
  }
  break;

  case SpecialPiece::CLEARLINE:
  {
    for (uCoord x = 0; x < fieldWidth; ++x)
      for (uCoord y = fieldHeight - 1; y < fieldHeight; --y)
        transform(x, y) = y == 0 ? FieldElement::NONE : field.get()(x, y - 1);
  }
  break;

  case SpecialPiece::NUKE:
  {
    for (uCoord i = 0; i < fieldSize; ++i)
      transform(i) = FieldElement::NONE;
  }
  break;

  case SpecialPiece::RANDOMCLEAR:
  {
    for (unsigned i = 0; i < 10; ++i)
      transform(getRand(fieldSize)) = FieldElement::NONE;
  }
  break;

  case SpecialPiece::SWITCH:
  {
    for (uCoord i = 0; i < fieldSize; ++i)
      transform(i) = target.get()(i);
  }
  break;

  case SpecialPiece::CLEARSPECIALS:
  {
    for (uCoord i = 0; i < fieldSize; ++i)
      if (isSpecial(field.get()(i)))
        transform(i) = FieldElement::RED;
  }
  break;

  case SpecialPiece::GRAVITY:
  {
    std::array<FieldElement, fieldHeight> tempCol;
    uCoord n = 0;
    for (uCoord x = 0; x < fieldWidth; ++x)
    {
      tempCol.fill(FieldElement::NONE);
      n = 0;
      for (uCoord y = 0; y < fieldHeight; ++y)
        if (field.get()(x, y) != FieldElement::NONE)
          tempCol[n++] = field.get()(x, y);
      for (uCoord y = 0; y < fieldHeight; ++y)
        transform(x, y) = tempCol[fieldHeight - y - 1];
    }
  }
  break;

  case SpecialPiece::QUAKE:
  {
    for (uCoord y = 0; y < fieldHeight; ++y)
    {
      sCoord s = 0;
      uCoord i = getRand(fieldHeight);
      if (i < 1) s++;
      if (i < 4) s++;
      if (i < 11) s++;
      if (getRand(2)) s *= -1;
      if (s > 0) // right
      {
        for (uCoord x = 1; x < fieldWidth; ++x)
          transform(x, y) = field.get()(x - 1, y);
        transform(0, y) = field.get()(fieldWidth - 1, y);
      }
      else if (s < 0) // left
      {
        for (uCoord x = fieldWidth - 1 - 1; x < fieldWidth; --x)
          transform(x, y) = field.get()(x + 1, y);
        transform(fieldWidth - 1, y) = field.get()(0, y);
      }
    }
  }
  break;

  case SpecialPiece::BOMB:
  {
    vector<FieldElement> scatters;
    for (uCoord y = 0; y < fieldHeight; ++y)
      for (uCoord x = 0; x < fieldWidth; ++x)
        if (field.get()(x, y) == FieldElement::BOMB)
        {
          transform(x, y) = FieldElement::NONE;
          for (uCoord i = 0; i < dp.size(); ++i)
          {
            if (!(boost::numeric_cast<uCoord>(x + dp[i].first)  < fieldWidth) &&
                !(boost::numeric_cast<uCoord>(y + dp[i].second) < fieldHeight))
              continue;
            FieldElement tmp = field.get()(x + dp[i].first, y + dp[i].second);
            if (tmp == FieldElement::BOMB)
              tmp = FieldElement::NONE;
            else
              transform(x + dp[i].first, y + dp[i].second) = FieldElement::NONE;
            scatters.push_back(tmp);
          }
        }
    for (unsigned i = 0; i < scatters.size(); ++i)
      transform(getRand(fieldWidth), getRand(fieldHeight - 6) + 6) = scatters[i];
  }
  break;
  
  }

  return transform;
}
