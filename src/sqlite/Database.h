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

    sqlite3* getHandle() const
    {
        checkIsOpenedAndThrowException();
        return handle;
    }

    void open(const QString &fileName);
    void create(const QString &fileName);
    void close();
    void executeSimpleQuery(const QString &queryText);
    void vacuum();

    bool isOpened() const
        { return handle != NULL; }

    ProgressHandler progressHandler;
    Tables tables;

private:
    void checkIsOpenedAndThrowException() const;

    sqlite3 *handle;
};

}

#endif // DATABASE_H
