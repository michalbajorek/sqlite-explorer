#include "Api.h"
#include "Database.h"
#include "Table.h"

using namespace sqlite;

Database::Database() : progressHandler(this), tables(this)
{
    handle = nullptr;
}

Database::~Database()
{
    if(isOpened())
        close();
}

void Database::internalOpen(const QString &fileName, OpenMode openMode)
{
    checkIsNotOpened();
    int flags = getOpenFlags(openMode);
    handle = Api::open(fileName, flags);
    if(openMode == OpenMode::Open)
        tables.load();
}

int Database::getOpenFlags(OpenMode openMode) const
{
    int flags = SQLITE_OPEN_READWRITE;
    if(openMode == OpenMode::Create)
        flags |= SQLITE_OPEN_CREATE;
    return flags;
}

void Database::close()
{
    checkIsOpened();
    tables.clear();
    Api::close(handle);
    handle = nullptr;
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
        throw common::Exception("Database is not opened");
}

void Database::checkIsNotOpened() const
{
    if(isOpened())
        throw common::Exception("Database is alredy opened");
}
