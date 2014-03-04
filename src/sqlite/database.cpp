#include "database.h"
#include "table.h"

using namespace sqlite;

Database::Database() : progressHandler(this), tables(this)
{
    this->handle = NULL;
}

Database::~Database()
{
    if(isOpened())
        close();
}

void Database::open(const QString &fileName)
{
    if(isOpened())
        throw Exception("Database alredy opened");
    int errorCode = sqlite3_open_v2(fileName.toUtf8(), &handle, SQLITE_OPEN_READWRITE, NULL);
    checkErrorCodeAndThrowException(errorCode);
    tables.loadTables();
}

void Database::create(const QString &fileName)
{
    if(isOpened())
        throw Exception("Database alredy opened");
    int errorCode = sqlite3_open_v2(fileName.toUtf8(), &handle, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL);
    checkErrorCodeAndThrowException(errorCode);
}

void Database::close()
{
    int errorCode = sqlite3_close_v2(getHandle());
    checkErrorCodeAndThrowException(errorCode);
}

void Database::executeSimpleQuery(const QString &queryText)
{
    int errorCode = sqlite3_exec(getHandle(), queryText.toUtf8(), NULL, NULL, NULL);
    checkErrorCodeAndThrowException(errorCode);
}

void Database::vacuum()
{
    executeSimpleQuery("VACUUM");
}

void Database::checkIsOpenedAndThrowException()
{
    if(!isOpened())
        throw Exception("Database is not opened");
}
