#include "Server.hpp"

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WBreak>

using namespace std;
using namespace Wt;

DeepThoughtWeb::DeepThoughtWeb(const WEnvironment& env)
  : WApplication(env),
    pop(new PopulationWidget("", root()))
{
  setTitle("DeepThought Web Control Panel");
}

WApplication* DeepThoughtWeb::CreateApplication(const WEnvironment& env)
{
  return new DeepThoughtWeb(env);
}

int main(int argc, char** argv)
{
  return WRun(argc, argv, &DeepThoughtWeb::CreateApplication);
}
