#ifndef TAB_H
#define TAB_H

#include <QString>

namespace ribbon
{

class Tabs;

class Tab
{
public:
    Tab(Tabs *tabs);

    const QString &getCaption()
        { return caption; }

    void setCaption(const QString &newCaption);

private:
    Tabs *tabs;
    QString caption;
};

} // namespace ribbon

#endif // TAB_H
