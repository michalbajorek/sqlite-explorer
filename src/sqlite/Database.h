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
    enum class OpenMode
    {
        Open,
        Create,
    };
public:
    Database();
    ~Database();

    sqlite3* getHandle() const
    {
        checkIsOpened();
        return handle;
    }

    void open(const QString &fileName)
        { internalOpen(fileName, OpenMode::Open); }

    void create(const QString &fileName)
        { internalOpen(fileName, OpenMode::Create); }

    void close();
    void executeSimpleQuery(const QString &queryText);
    void vacuum();

    bool isOpened() const
        { return handle != NULL; }

    ProgressHandler progressHandler;
    Tables tables;

private:
    int getOpenFlags(OpenMode openMode);
    void checkIsOpened() const;
    void checkIsNotOpened() const;
    void internalOpen(const QString &fileName, OpenMode openMode);

    sqlite3 *handle;
};

}

#endif // DATABASE_H
