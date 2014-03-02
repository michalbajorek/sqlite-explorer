#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSet>
#include <QMap>

#include "exception.h"
#include "object.h"
#include "progress_handler.h"
#include "sqlite3.h"

namespace sqlite
{

class Table;

class Database
{

public:
    Database();
    ~Database();

    sqlite3* getHandle()
        { return handle; }

    void open(const QString &fileName);
    void close();
    void executeSimpleQuery(const QString &queryText);
    void vacuum();

    Table* getTable(const QString &name);
    Table* getMasterTable();

    ProgressHandler progressHandler;

private:
    sqlite3 *handle;
    QMap<QString, Table> tables;
};

}

#endif // DATABASE_H
