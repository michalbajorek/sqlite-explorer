#include <QPainter>

#include "ribbon.h"

using namespace ribbon;

Ribbon::Ribbon(QWidget *parent) : QWidget(parent)
{
    setMinimumHeight(100);
}


void ribbon::Ribbon::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.fillRect(this->rect(), Qt::red);
}
