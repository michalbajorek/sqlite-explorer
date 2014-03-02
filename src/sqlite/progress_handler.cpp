#include "database.h"
#include "progress_handler.h"
#include "sqlite3.h"

using namespace sqlite;


ProgressObserver::ProgressObserver()
{
    progressHandler = NULL;
}

ProgressObserver::~ProgressObserver()
{
    if(progressHandler)
        progressHandler->removeObserver(this);
}

ProgressHandler::ProgressHandler(Database *database) : Object(database)
{
    operationInterval = 10;
}

ProgressHandler::~ProgressHandler()
{
    removeAllObservers();
}

void ProgressHandler::addObserver(ProgressObserver *observer)
{
    observersSet.insert(observer);
    observer->progressHandler = this;
    setHandler();
}

void ProgressHandler::removeObserver(ProgressObserver *observer)
{
    observersSet.remove(observer);
    observer->progressHandler = NULL;
    if(observersSet.empty())
        removeHandler();
}

void ProgressHandler::setOperationInterval(int operationInterval)
{
    this->operationInterval = operationInterval;
    setHandler();
}

void ProgressHandler::setHandler()
{
    sqlite3_progress_handler(database->getHandle(), operationInterval, staticProgressHandler, this);
}

void ProgressHandler::removeHandler()
{
    sqlite3_progress_handler(database->getHandle(), 0, NULL, NULL);
}

int ProgressHandler::staticProgressHandler(void *param)
{
    ProgressHandler *progressHandler = static_cast<ProgressHandler*>(param);
    return progressHandler->notifyObservers();
}

bool ProgressHandler::notifyObservers()
{
    bool cancelOperation = false;
    foreach(ProgressObserver *observer, observersSet)
    {
        observer->onProgressHandler(database, cancelOperation);
        if(cancelOperation)
            break;
    }
    return cancelOperation;
}

void ProgressHandler::removeAllObservers()
{
    foreach(ProgressObserver *observer, observersSet)
        removeObserver(observer);
}
