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

    int getCount() const
        { return tableMap.count(); }

    Table* getTable(const QString &tableName) const;
    Table* getTable(int index) const;
    Table* getMasterTable() const
        { return masterTable; }

private:
    void load();
    void clear();
    void addTable(const QString &tableName);
    void loadMasterTable();
    void loadRestTables();
    void clearMasterTable();
    void clearRestTables();

    QMap<QString, Table*> tableMap;
    Table *masterTable;

    friend class Database;
};

}// namespace sqlite

#endif // TABLES_H
