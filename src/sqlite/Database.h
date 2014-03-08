#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

#include "Exception.h"
#include "Object.h"
#include "ProgressHandler.h"
#include "sqlite3.h"
#include "Tables.h"

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
        checkIsOpened();
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
    void checkIsOpened() const;
    void checkIsNotOpened() const;

    sqlite3 *handle;
};

}

#endif // DATABASE_H
