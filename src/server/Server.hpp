#ifndef SERVER_HPP
#define SERVER_HPP

#include <Wt/WApplication>

#include "PopulationWidget.hpp"

class DeepThoughtWeb : public Wt::WApplication
{
public:
  DeepThoughtWeb(const Wt::WEnvironment& env);

  static Wt::WApplication* CreateApplication(const Wt::WEnvironment& env);
private:
  PopulationWidget* pop;
};

#endif
