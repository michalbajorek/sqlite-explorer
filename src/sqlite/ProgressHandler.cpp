﻿#include "Api.h"
#include "Database.h"
#include "ProgressHandler.h"

using namespace sqlite;

ProgressHandler::ProgressHandler(Database *database) : Object(database)
{
    operationInterval = 50;
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

void ProgressHandler::setOperationInterval(int newOperationInterval)
{
    operationInterval = newOperationInterval;
    setHandler();
}

void ProgressHandler::setHandler()
{
    Api::progressHandler(database->getHandle(), operationInterval, staticProgressHandler, this);
}

void ProgressHandler::removeHandler()
{
    Api::progressHandler(database->getHandle(), 0, nullptr, nullptr);
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
