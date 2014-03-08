#include "Application.h"

#ifndef TESTCASE

int main(int argc, char *argv[])
{
    Application application(argc, argv);
    application.initialize();
    application.createMainWindow();
    return application.run();
}

#endif // TESTCASE
