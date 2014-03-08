#ifndef TABLE_H
#define TABLE_H

#include "RecordSet.h"

namespace sqlite
{

class Database;

class Table : public RecordSet
{
public:
    Table(Database *database, const QString &name);
    ~Table();

    void loadContent()
        { setQueryText("SELECT * FROM " + name); }

     const QString &getName() const
        { return name; }

private:
    QString name;
};

} // namespace sqlite

#endif // TABLE_H
