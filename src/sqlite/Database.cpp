#include "Api.h"
#include "Database.h"
#include "Table.h"

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
    checkIsNotOpened();
    handle = Api::open(fileName, SQLITE_OPEN_READWRITE);
    tables.load();
}

void Database::create(const QString &fileName)
{
    checkIsNotOpened();
    handle = Api::open(fileName, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
}

void Database::close()
{
    tables.clear();
    Api::close(getHandle());
    handle = NULL;
}

void Database::executeSimpleQuery(const QString &queryText)
{
    Api::exec(getHandle(), queryText);
}

void Database::vacuum()
{
    Api::exec(getHandle(), "VACUUM");
}

void Database::checkIsOpened() const
{
    if(!isOpened())
        throw Exception("Database is not opened");
}

void Database::checkIsNotOpened() const
{
    if(isOpened())
        throw Exception("Database is alredy opened");
}
