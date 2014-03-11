#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QSplitter>

class Settings
{
public:
    Settings();

    void loadWindowGeometry(QWidget *widget);
    void saveWindowGeometry(QWidget *widget);

    void loadSplitterState(QSplitter *splitter);
    void saveSplitterState(QSplitter *splitter);

private:
    QSettings settings;
};

extern Settings globalSettings;

#endif // SETTINGS_H
