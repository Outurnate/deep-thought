#include "libtetrinet/GameSettings.hpp"

GameSettings::NumberCache GameSettings::rngCache = GameSettings::NumberCache();

GameSettings::GameSettings(unsigned startHeight, unsigned startLevel, unsigned lineLevel, unsigned levelIncr,
			   unsigned lineSpecial, unsigned specialCount, unsigned specialCap, std::string blockFrequency,
			   std::string specialFrequency, bool showAvgLevel, bool classic, unsigned seed)
    : startHeight(startHeight), startLevel(startLevel), lineLevel(lineLevel), levelIncr(levelIncr),
      lineSpecial(lineSpecial), specialCount(specialCount), specialCap(specialCap), seed(seed),
      blockFrequency(blockFrequency), specialFrequency(specialFrequency), showAvgLevel(showAvgLevel),
      classic(classic), pieceNum(0), specialNum(0)
{
}

Piece GameSettings::GetPiece()
{
  size_t num = pieceNum++;
  return Piece::Get(PieceShape(blockFrequency[rng(seed, num) % blockFrequency.size()]),
		    PieceRotation(rng(seed, num) % 4));
}

uint32_t GameSettings::mcg(uint32_t prev)
{
  static uint32_t a = 0x08088405,
    c = 1;
  return (a * prev + c); // no need to mod M, integer overflow will handle it
}

uint32_t GameSettings::rng(uint32_t s, size_t n)
{
  if (n > rngCache[s].size())
  {
    size_t orig = rngCache[s].size(),
      diff = n - orig;
    rngCache[s].resize(orig + diff);
    for (size_t i = 0; i < diff; ++i)
      rngCache[s][i + orig] = mcg(orig == 0 ? s : rngCache[s][(i - 1) + orig]);
  }

  return n == 0 ? s : rngCache[s][n - 1];
}
