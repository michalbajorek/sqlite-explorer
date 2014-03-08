#include "Database.h"
#include "Table.h"

using namespace sqlite;

Table::Table(Database *database, const QString &name)
: RecordSet(database)
{
    this->name = name;
}

Table::~Table()
{

}
