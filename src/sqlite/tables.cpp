#include "database.h"
#include "recordset.h"
#include "table.h"
#include "tables.h"

using namespace sqlite;

Tables::Tables(Database *database) : Object(database)
{

}

Tables::~Tables()
{

}

void Tables::loadTables()
{
    RecordSet getTables(database, "SELECT name FROM sqlite_master WHERE type = 'table'");
    for(int index = 0; index < getTables.getRecordsCount(); index++)
    {
        Record &record = getTables.getRecord(index);
        addTable(record[0]);
    }
}

Table* Tables::getTable(const QString &tableName)
{
    Table *table = tableMap[tableName];
    if(table == NULL)
        throw Exception("Table " + tableName + " does not exist");
    return table;
}

Table *Tables::getTable(int index)
{
    auto iterator = tableMap.begin();
    return (iterator + index).value();
}

void Tables::addTable(const QString &tableName)
{
    Table *table = new Table(database, tableName);
    tableMap.insert(tableName, table);
}
