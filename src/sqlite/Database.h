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

    void open(const QString &fileName)
        { internalOpen(fileName, OpenMode::Open); }

    void create(const QString &fileName)
        { internalOpen(fileName, OpenMode::Create); }

    void close();

    bool isOpened() const
        { return handle != NULL; }

    sqlite3* getHandle() const
    {
        checkIsOpened();
        return handle;
    }

    void executeSimpleQuery(const QString &queryText);
    void vacuum();

    ProgressHandler progressHandler;
    Tables tables;

private:
    enum class OpenMode
    {
        Open,
        Create,
    };

    int getOpenFlags(OpenMode openMode);
    void checkIsOpened() const;
    void checkIsNotOpened() const;
    void internalOpen(const QString &fileName, OpenMode openMode);

    sqlite3 *handle;
};

}

#endif // DATABASE_H
