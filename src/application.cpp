#include <QApplication>

#include "application.h"
#include "gui_main_window.h"

Application::Application(int argumentCount, char *argumentTable[])
{
    qtApplication = new QApplication(argumentCount, argumentTable);
    mainWindow = NULL;
}

Application::~Application()
{
    delete mainWindow;
    delete qtApplication;
}

void Application::initialize()
{
    initialization.setUtf8EncodingForCStrings();
}

void Application::createMainWindow()
{
    mainWindow = new MainWindow();
    mainWindow->show();
}

int Application::run()
{
    return qtApplication->exec();
}
