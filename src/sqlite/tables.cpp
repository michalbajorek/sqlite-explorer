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

void Tables::load()
{
    RecordSet getTables(database, "SELECT name FROM sqlite_master WHERE type = 'table'");
    for(int index = 0; index < getTables.getRecordsCount(); index++)
    {
        const Record &record = getTables.getRecord(index);
        addTable(record.getField(0));
    }
}

void Tables::clear()
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
    if(table == NULL)
        throw Exception("Table " + tableName + " does not exist");
    return table;
}

Table *Tables::getTable(int index) const
{
    auto iterator = tableMap.begin() + index; // Iterator has operator+
    return iterator.value();
}

void Tables::addTable(const QString &tableName)
{
    Table *table = new Table(database, tableName);
    tableMap.insert(tableName, table);
}
