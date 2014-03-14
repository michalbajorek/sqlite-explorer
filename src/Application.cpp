#include <QMessageBox>

#include "Application.h"
#include "MainWindow.h"

Application::Application(int argumentCount, char *argumentTable[])
: QApplication(argumentCount, argumentTable)
{
    mainWindow = nullptr;
}

Application::~Application()
{
    delete mainWindow;
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
try
{
    return exec();
}
catch(common::Exception &exception)
{
    if(showQuitDialog(exception.getErrorMessage()))
        quit();
    return -1;
}
catch(...)
{
    if(showQuitDialog("Unhandled exception"))
        quit();
    return -1;
}

bool Application::notify(QObject *receiver, QEvent *event)
{
    try
    {
        return QApplication::notify(receiver, event);
    }
    catch(common::Exception &exception)
    {
        if(showQuitDialog(exception.getErrorMessage()))
            quit();
    }
    catch(...)
    {
        if(showQuitDialog("Unhandled exception"))
            quit();
    }
    return true;
}

bool Application::showQuitDialog(const QString &errorMessage)
{
    QMessageBox messageBox;
    messageBox.setIcon(QMessageBox::Critical);
    messageBox.setWindowTitle("Error");
    messageBox.setText(errorMessage);
    messageBox.addButton("Continue", QMessageBox::RejectRole);
    messageBox.addButton("Quit application", QMessageBox::AcceptRole);
    return messageBox.exec() == QDialog::Accepted;
}

