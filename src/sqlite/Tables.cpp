#include "Database.h"
#include "RecordSet.h"
#include "Table.h"
#include "Tables.h"

using namespace sqlite;

Tables::Tables(Database *database) : Object(database)
{
    masterTable = nullptr;
}

Tables::~Tables()
{
    clear();
}

void Tables::load()
{
    loadMasterTable();
    loadRestTables();
}

void Tables::loadMasterTable()
{
    masterTable = new Table(database, "sqlite_master");
}

void Tables::loadRestTables()
{
    RecordSet getTables(database);
    getTables.setQueryText("SELECT name FROM sqlite_master WHERE type = 'table'");
    for(int index = 0; index < getTables.getRecordsCount(); index++)
    {
        const Record &record = getTables.getRecord(index);
        addTable(record.getField(0));
    }
}

void Tables::addTable(const QString &tableName)
{
    Table *table = new Table(database, tableName);
    tableMap.insert(tableName, table);
}

void Tables::clear()
{
    clearMasterTable();
    clearRestTables();
}

void Tables::clearMasterTable()
{
    delete masterTable;
    masterTable = nullptr;
}

void Tables::clearRestTables()
{
    foreach(auto it, tableMap)
    {
        Table *table = it;
        delete table;
    }
    tableMap.clear();
}

Table* Tables::getTable(const QString &tableName) const
{
    Table *table = tableMap[tableName];
    if(table == nullptr)
        throw common::Exception("Table " + tableName + " does not exist");
    return table;
}

Table *Tables::getTable(int index) const
{
    auto iterator = tableMap.begin() + index; // Iterator has operator+
    return iterator.value();
}
