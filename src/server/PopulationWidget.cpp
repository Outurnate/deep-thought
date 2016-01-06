#include "PopulationWidget.hpp"

#include <Wt/WTable>
#include <Wt/WText>

using namespace std;
using namespace Wt;

PopulationWidget::PopulationWidget(const string& name, WContainerWidget* parent)
  : WContainerWidget(parent),
    populationTable(new WTable(this))
{
  populationTable->setHeaderCount(1);
  populationTable->insertColumn(0);
  populationTable->insertColumn(1);
  populationTable->insertColumn(2);
  WTableRow* header = populationTable->insertRow(0);
  header->elementAt(0)->addWidget(new WText("Population Name", this));
  header->elementAt(1)->addWidget(new WText("Generations", this));
  header->elementAt(2)->addWidget(new WText("Manage", this));
  this->addWidget(populationTable);
}

void PopulationWidget::reloadData()
{
  populationTable->clear();
}
