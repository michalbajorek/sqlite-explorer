#include <QWidget>
#include "Settings.h"

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

void Settings::loadSplitterState(QSplitter *splitter)
{
    QByteArray state = settings.value(splitter->objectName() + "State").toByteArray();
    splitter->restoreState(state);
}

void Settings::saveSplitterState(QSplitter *splitter)
{
    QByteArray state = splitter->saveState();
    settings.setValue(splitter->objectName() + "State", state);
}
