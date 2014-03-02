#include "database.h"
#include "table.h"
#include "tables.h"

using namespace sqlite;

Tables::Tables(Database *database) : Object(database)
{
}
