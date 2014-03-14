#include "DatabaseRepository.h"

using namespace sqlite;

QHash<QString, Database*> DatabaseRepository::databaseHash;

Database *DatabaseRepository::get(const QString &fileName)
{
    if(isOpened(fileName))
        return databaseHash[fileName];
    Database *database = createAndOpenDatabase(fileName);
    databaseHash.insert(fileName, database);
    return database;
}

Database * DatabaseRepository::createAndOpenDatabase(const QString &fileName)
{
    Database *database = new Database();
    database->open(fileName);
    return database;
}

void DatabaseRepository::release(Database *database)
{
    QString fileName = database->getFileName();
    if(isOpened(fileName) == false)
        throw Exception("Database not exists in repository");
    databaseHash.remove(fileName);
    delete database;
}

bool DatabaseRepository::isOpened(const QString &fileName)
{
    return databaseHash.contains(fileName);
}
