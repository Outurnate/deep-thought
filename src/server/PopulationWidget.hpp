#ifndef POPULATIONWIDGET_HPP
#define POPULATIONWIDGET_HPP

#include <Wt/WContainerWidget>

class PopulationWidget : public Wt::WContainerWidget
{
public:
  PopulationWidget(const std::string& name, Wt::WContainerWidget* parent = 0);
private:
  void reloadData();
  
  Wt::WTable* populationTable;
};

#endif
