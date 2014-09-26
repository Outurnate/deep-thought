//============================================================================
// Name        : DeepThought.cpp
// Author      : josephd
// Version     :
// Description : a thing
//============================================================================

#include "AIEngine.h"

int main (/*int argc, const char* argv[]*/)
{
	boost::asio::io_service srv;
	AIEngine(srv, "DeepThought", -5.f, -10.f, -.5f, 20.f).Run();

	return 0;
}
