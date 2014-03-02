#include "database.h"
#include "table.h"

using namespace sqlite;

Database::Database() : progressHandler(this)
{
    this->handle = NULL;
}

Database::~Database()
{
    close();
}

void Database::open(const QString &fileName)
{
    int errorCode = sqlite3_open_v2(fileName.toUtf8(), &handle, SQLITE_OPEN_READWRITE, NULL);
    checkErrorCodeAndThrowException(errorCode);
}

void Database::close()
{
    int errorCode = sqlite3_close_v2(handle);
    checkErrorCodeAndThrowException(errorCode);
}

void Database::executeSimpleQuery(const QString &queryText)
{
    int errorCode = sqlite3_exec(handle, queryText.toUtf8(), NULL, NULL, NULL);
    checkErrorCodeAndThrowException(errorCode);
}

void Database::vacuum()
{
    executeSimpleQuery("VACUUM");
}

Table* Database::getTable(const QString &name)
{
    return new Table(this, name);
}

Table *Database::getMasterTable()
{
    return getTable("sqlite_master");
}
