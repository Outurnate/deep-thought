#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// Compiler magic

#define FIELD_WIDTH       12
#define FIELD_HEIGHT      22
#define FIELD_SIZE        (FIELD_WIDTH * FIELD_HEIGHT)

#define PAIR_BORDER       1

#define CHANNEL_PREFIX    "dp-"
#define CHANNEL_SPECIFIER ((CHANNEL_PREFIX) + "ch")

unsigned long constexpr djb2(const char* c);

unsigned long constexpr djb2(const char* c, unsigned long hash)
{
  return !*c ? hash : djb2(c + 1, ((hash << 5) + hash) + *c);
}

unsigned long constexpr djb2(const char* c)
{
  return djb2(c, 5381);
}

#endif
