#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings
{
public:
    Settings();

    void loadWindowGeometry(QWidget *widget);
    void saveWindowGeometry(QWidget *widget);

private:
    QSettings settings;
};

extern Settings globalSettings;

#endif // SETTINGS_H
