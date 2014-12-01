/*
 * DeepThought.cpp
 *
 *  Created on: Oct 26, 2012
 *      Author: joseph
 */

#include "TermInterface.hpp"
#include "AIEngine.hpp"

int main ()
{
  AIEngine engine("DeepThought", -5.f, -5.f, -7.f, 20.f);
  TermInterface interface;
  interface.engines.push_back(engine);

  interface.Show();
  return 0;
}
