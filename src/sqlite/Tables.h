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

private:
    void load();
    void clear();
    void addTable(const QString &tableName);

    QMap<QString, Table*> tableMap;

    friend class Database;
};

}// namespace sqlite

#endif // TABLES_H
