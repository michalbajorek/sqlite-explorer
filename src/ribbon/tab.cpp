#include "tab.h"
#include "tabs.h"

using namespace ribbon;

Tab::Tab(Tabs *tabs)
{
    this->tabs = tabs;
}

void Tab::setCaption(const QString &newCaption)
{
    caption = newCaption;
    tabs->updateView();
}
