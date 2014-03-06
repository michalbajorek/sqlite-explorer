#ifndef RIBBON_H
#define RIBBON_H

#include <QWidget>
#include <QPainter>

#include "tabs.h"

namespace ribbon
{

class Ribbon : public QWidget
{
public:
    Ribbon(QWidget *parent = NULL);

    Tabs tabs;

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
};

} // namespace ribbon

#endif // RIBBON_H
