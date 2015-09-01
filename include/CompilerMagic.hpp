#ifndef COMPILER_MAGIC
#define COMPILER_MAGIC

#include <string>
#include <algorithm>

unsigned long constexpr djb2(const char* c);

unsigned long constexpr djb2(const char* c, unsigned long hash)
{
  return !*c ? hash : djb2(c + 1, ((hash << 5) + hash) + *c);
}

unsigned long constexpr djb2(const char* c)
{
  return djb2(c, 5381);
}

const std::string stringToLower(const std::string& str)
{
  std::string ret(str);
  std::transform(str.begin(), str.end(), ret.begin(), ::tolower);
  return ret;
}

#define TOCHAR(c) ((#c)[0])

#endif
