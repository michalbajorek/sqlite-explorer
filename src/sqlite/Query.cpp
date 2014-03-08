#include <cassert>

#include "Api.h"
#include "Database.h"
#include "Query.h"

using namespace sqlite;

Query::Query(Database *database) : Object(database)
{
    init();
}

Query::Query(Database *database, const QString &query) : Object(database)
{
    prepare(query);
}

Query::~Query()
{
    finalize();
}

void Query::init()
{
    lastStepResult = SQLITE_OK;
    statement = NULL;
}

void Query::prepare(const QString &query)
{
    init();
    statement = Api::prepare(database->getHandle(), query);
}

bool Query::step()
{
    lastStepResult = Api::step(statement);
    return lastStepResult;
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
