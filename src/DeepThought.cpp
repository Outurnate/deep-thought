//============================================================================
// Name        : DeepThought.cpp
// Author      : josephd
// Version     :
// Description : a thing
//============================================================================

#include "AIEngine.hpp"

int main (/*int argc, const char* argv[]*/)
{
  boost::asio::io_service srv;
  AIEngine(srv, "DeepThought", 10.f, 10.f, 0.f, 20.f).Run();

  return 0;
}
