#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSet>
#include <QMap>

#include "exception.h"
#include "object.h"
#include "progress_handler.h"
#include "sqlite3.h"
#include "tables.h"

namespace sqlite
{

class Table;

class Database
{

public:
    Database();
    ~Database();

    sqlite3* getHandle()
    {
        checkIsOpenedAndThrowException();
        return handle;
    }

    void open(const QString &fileName);
    void create(const QString &fileName);
    void close();
    void executeSimpleQuery(const QString &queryText);
    void vacuum();

    bool isOpened()
        { return handle != NULL; }

    Table* getTable(const QString &name);
    Table* getMasterTable();

    ProgressHandler progressHandler;
    Tables tables;

private:
    void checkIsOpenedAndThrowException();

    sqlite3 *handle;
};

}

#endif // DATABASE_H
