#ifndef TABS_H
#define TABS_H

#include <QPainter>
#include <QList>
#include <QString>

namespace ribbon
{

class Tab;

class Tabs
{
public:
    Tabs();

    Tab* addTab(const QString &caption);

    void draw(QPainter &painter);

private:
    void updateView();

    QList<Tab*> tabList;

    friend class Tab;
};

}// namespace ribbon

#endif // TABS_H
