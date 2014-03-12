#include "DatabaseList.h"
#include "sqlite/Database.h"


DatabaseList::DatabaseList()
{
}

DatabaseList::~DatabaseList()
{

}

void DatabaseList::addAndOpenDatabase(const QString &fileName)
{
    sqlite::Database *database = new sqlite::Database();
    database->open(fileName);
    databaseList.append(database);
    model.addDatabase(database);
}
