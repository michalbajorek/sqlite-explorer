﻿#include "database.h"
#include "table.h"

using namespace sqlite;

Table::Table(Database *database, const QString &name)
: RecordSet(database)
{
    this->name = name;
    setQueryText("SELECT * FROM " + name);
}

Table::~Table()
{

}
