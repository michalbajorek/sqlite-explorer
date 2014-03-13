#include "DatabaseList.h"
#include "sqlite/Database.h"


DatabaseList::DatabaseList()
{
    isUpdating = false;
}

DatabaseList::~DatabaseList()
{

}

void DatabaseList::addDatabase(const QString &fileName)
{
    if(isDatabaseOpened(fileName))
        return;
    sqlite::Database *database = createAndOpenDatabase(fileName);
    databaseHash.insert(fileName, database);
    updateModel();
}

bool DatabaseList::isDatabaseOpened(const QString &fileName)
{
    return databaseHash.contains(fileName);
}

sqlite::Database * DatabaseList::createAndOpenDatabase(const QString &fileName)
{
    sqlite::Database *database = new sqlite::Database;
    database->open(fileName);
    return database;
}

void DatabaseList::updateModel()
{
    if(isUpdating == false)
        model.setDatabaseHash(databaseHash);
}

void DatabaseList::removeDatabase(const QString &fileName)
{
    if(isDatabaseOpened(fileName) == false)
        throw sqlite::Exception("Database is not opened");
    closeAndDeleteDatabase(fileName);
    databaseHash.remove(fileName);
    updateModel();
}

void DatabaseList::closeAndDeleteDatabase(const QString &fileName)
{
    sqlite::Database *database = databaseHash[fileName];
    delete database;
}

void DatabaseList::beginUpdate()
{
    isUpdating = true;
}

void DatabaseList::endUpdate()
{
    isUpdating = false;
    updateModel();
}

