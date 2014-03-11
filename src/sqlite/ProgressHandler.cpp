#include "Api.h"
#include "Database.h"
#include "ProgressHandler.h"

using namespace sqlite;

ProgressHandler::ProgressHandler(Database *database) : Object(database)
{
    operationInterval = 10;
    enabled = false;
}

ProgressHandler::~ProgressHandler()
{
    if(enabled && database->isOpened())
        removeHandler();
}

void ProgressHandler::setEnabled(bool newEnabled)
{
    enabled = newEnabled;
    if(enabled)
        setHandler();
    else
        removeHandler();
}

void ProgressHandler::setOperationInterval(int operationInterval)
{
    this->operationInterval = operationInterval;
    setHandler();
}

void ProgressHandler::setHandler()
{
    Api::progressHandler(database->getHandle(), operationInterval, staticProgressHandler, this);
}

void ProgressHandler::removeHandler()
{
    Api::progressHandler(database->getHandle(), 0, NULL, NULL);
}

int ProgressHandler::staticProgressHandler(void *param)
{
    ProgressHandler *progressHandler = static_cast<ProgressHandler*>(param);
    return progressHandler->emitProgressSignal();
}

bool ProgressHandler::emitProgressSignal()
{
    bool cancelOperation = false;
    emit progress(database, cancelOperation);
    return cancelOperation;
}
