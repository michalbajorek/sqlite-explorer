#include <cassert>

#include "Api.h"
#include "Database.h"
#include "Query.h"
#include "QueryLogger.h"
#include <QDebug>

using namespace sqlite;

Query::Query(Database *database) : Object(database)
{
    init();
}

Query::~Query()
{
    if(isActive())
        finalize();
}

void Query::init()
{
    isInProgress = false;
    statement = nullptr;
}

void Query::prepare(const QString &query)
{
    init();
    statement = Api::prepare(database->getHandle(), query);
}

bool Query::step()
{
    isInProgress = Api::step(statement);
    return isInProgress;
}

void Query::reset()
{
    Api::reset(statement);
}

void Query::finalize()
{
    Api::finalize(statement);
    init();
}
