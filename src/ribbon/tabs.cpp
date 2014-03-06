#include "tab.h"
#include "tabs.h"

using namespace ribbon;

Tabs::Tabs()
{
}

Tab *Tabs::addTab(const QString &caption)
{
    Tab *newTab = new Tab(this);
    newTab->setCaption(caption);
    tabList.append(newTab);
    return newTab;
}

void Tabs::updateView()
{

}
