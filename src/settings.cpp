#include <QWidget>
#include "settings.h"

Settings globalSettings;

Settings::Settings()
: settings("sql", "sqlite-explorer")
{

}

void Settings::loadWindowGeometry(QWidget *widget)
{
    QByteArray geometry = settings.value(widget->objectName() + "Geometry").toByteArray();
    widget->restoreGeometry(geometry);
}

void Settings::saveWindowGeometry(QWidget *widget)
{
    QByteArray geometry = widget->saveGeometry();
    settings.setValue(widget->objectName() + "Geometry", geometry);
}
