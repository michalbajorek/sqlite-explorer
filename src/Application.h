#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include "Initialization.h"

class MainWindow;
class QApplication;

class Application : protected QApplication
{
public:
    Application(int argumentCount, char *argumentTable[]);
    ~Application();

    void initialize();
    void createMainWindow();
    int run();

private:
    bool showQuitDialog(const QString &errorMessage);
    virtual bool notify(QObject *receiver, QEvent *event);

    Initialization initialization;
    MainWindow *mainWindow;


};

#endif // APPLICATION_H
