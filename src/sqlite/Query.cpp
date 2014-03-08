#include <cassert>

#include "database.h"
#include "query.h"

using namespace sqlite;

Query::Query(Database *database) : Object(database)
{
    init();
}

Query::Query(Database *database, const QString &queryText) : Object(database)
{
    prepare(queryText);
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

void Query::prepare(const QString &queryText)
{
    init();
    int errorCode = sqlite3_prepare_v2(database->getHandle(), queryText.toUtf8(), queryText.length(), &statement, NULL);
    checkErrorCodeAndThrowException(errorCode);
}

bool Query::step()
{
    lastStepResult = sqlite3_step(statement);
    switch(lastStepResult)
    {
    case SQLITE_DONE:
        return false;
    case SQLITE_ROW:
        return true;
    case SQLITE_BUSY:
    case SQLITE_ERROR:
    case SQLITE_MISUSE:
        checkErrorCodeAndThrowException(lastStepResult);
        break;
    default:
        assert(false);
    }
    return false;
}

void Query::reset()
{
    int errorCode = sqlite3_reset(statement);
    checkErrorCodeAndThrowException(errorCode);
}

void Query::finalize()
{
    int errorCode = sqlite3_finalize(statement);
    checkErrorCodeAndThrowException(errorCode);
    init();
}

void Query::bindInteger(int paramIndex, int value)
{
    int errorCode = sqlite3_bind_int(statement, paramIndex, value);
    checkErrorCodeAndThrowException(errorCode);
}

void Query::bindInt64(int paramIndex, int64_t value)
{
    int errorCode = sqlite3_bind_int64(statement, paramIndex, value);
    checkErrorCodeAndThrowException(errorCode);
}

void Query::bindDouble(int paramIndex, double value)
{
    int errorCode = sqlite3_bind_double(statement, paramIndex, value);
    checkErrorCodeAndThrowException(errorCode);
}

void Query::bindText(int paramIndex, const QString &value)
{
    int errorCode = sqlite3_bind_text(statement, paramIndex, value.toUtf8(), value.length(), NULL);
    checkErrorCodeAndThrowException(errorCode);
}
