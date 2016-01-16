#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>

#include "DeepThoughtForward.hpp"

struct Configuration
{
  std::string connectionString;
  unsigned generationSize = 100;
};

#endif
