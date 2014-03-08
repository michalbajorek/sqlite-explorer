#ifndef APPLICATION_H
#define APPLICATION_H

#include "Initialization.h"

class MainWindow;
class QApplication;

class Application
{
public:
    Application(int argumentCount, char *argumentTable[]);
    ~Application();

    void initialize();
    void createMainWindow();
    int run();


private:
    Initialization initialization;
    MainWindow *mainWindow;
    QApplication *qtApplication;

};

#endif // APPLICATION_H
