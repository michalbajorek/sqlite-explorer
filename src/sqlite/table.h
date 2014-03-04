#ifndef TABLE_H
#define TABLE_H

#include "recordset.h"

namespace sqlite
{

class Database;

class Table : public RecordSet
{
public:
    Table(Database *database, const QString &name);
    ~Table();

    QString getName()
        { return name; }

private:
    QString name;
};

} // namespace sqlite

#endif // TABLE_H
