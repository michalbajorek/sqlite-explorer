#include "database.h"
#include "progress_handler.h"
#include "sqlite3.h"

using namespace sqlite;

ProgressHandler::ProgressHandler(Database *database) : Object(database)
{
    this->function = NULL;
}

void ProgressHandler::set(Function function, int operationInterval)
{
    this->function = function;
    sqlite3_progress_handler(database->getHandle(), operationInterval, staticProgressHandler, this);
}

void ProgressHandler::remove()
{
    function = NULL;
    sqlite3_progress_handler(database->getHandle(), 0, NULL, NULL);
}

int ProgressHandler::staticProgressHandler(void *param)
{
    ProgressHandler *progressHandler = static_cast<ProgressHandler*>(param);
    return progressHandler->invokeFunction();
}

bool ProgressHandler::invokeFunction()
{
    bool cancelOperation = false;
    if(function)
        function(database, cancelOperation);
    return cancelOperation;
}
