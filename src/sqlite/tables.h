#ifndef TABLES_H
#define TABLES_H

#include<QMap>

namespace sqlite
{

class Database;
class Table;

class Tables : public Object
{
public:
    Tables(Database *database);
    virtual ~Tables();

    int getCount()
        { return tableMap.count(); }

    Table* getTable(const QString &tableName);
    Table* getTable(int index);


private:
    void loadTables();
    void addTable(const QString &tableName);

    QMap<QString, Table*> tableMap;

    friend class Database;
};

}// namespace sqlite

#endif // TABLES_H
