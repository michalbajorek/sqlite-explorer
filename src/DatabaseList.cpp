#include "DatabaseList.h"
#include "sqlite/DatabaseRepository.h"
#include "sqlite/Database.h"


DatabaseList::DatabaseList()
{
    isUpdating = false;
}

DatabaseList::~DatabaseList()
{
    removeAllDatabases();
}

void DatabaseList::removeAllDatabases()
{
    foreach(sqlite::Database *database, databaseList)
        delete database;
    databaseList.clear();
}

void DatabaseList::addDatabase(const QString &fileName)
{
    if(sqlite::DatabaseRepository::isOpened(fileName))
        return;
    sqlite::Database *database = sqlite::DatabaseRepository::get(fileName);
    databaseList.append(database);
    updateModel();
}

void DatabaseList::updateModel()
{
    if(isUpdating == false)
        model.setDatabaseList(databaseList);
}

void DatabaseList::removeDatabase(sqlite::Database *database)
{
    databaseList.removeOne(database);
    sqlite::DatabaseRepository::release(database);
    updateModel();
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

