#ifndef TAB_H
#define TAB_H

#include <QPainter>
#include <QString>

namespace ribbon
{

class Tabs;

class Tab
{
public:
    Tab(Tabs *tabs);

    const QString &getCaption() const
        { return caption; }

    void setCaption(const QString &newCaption);

    const QRect &getRect()
        { return rect; }

private:
    void draw(QPainter &painter);

    Tabs *tabs;
    QString caption;
    QRect rect;

    friend class Tabs;
};

} // namespace ribbon

#endif // TAB_H
