#ifndef SQLITEAPI_H
#define SQLITEAPI_H

#include <QString>

#include "sqlite3.h"
#include "Exception.h"

namespace sqlite
{

class Api
{
public:
    static sqlite3* open(const QString &fileName, int flags)
    {
        sqlite3 *handle;
        int result = sqlite3_open_v2(fileName.toUtf8(), &handle, flags, NULL);
        if(isError(result))
        {
            sqlite3_close(handle);
            throwException(result);
        }
        return handle;
    }

    static void close(sqlite3 *handle)
    {
        int result = sqlite3_close_v2(handle);
        if(isError(result))
            throwException(result);
    }

    static void exec(sqlite3 *handle, const QString &query)
    {
        int result = sqlite3_exec(handle, query.toUtf8(), NULL, NULL, NULL);
        if(isError(result))
            throwException(result);
    }

    static sqlite3_stmt* prepare(sqlite3 *handle, const QString &query)
    {
        sqlite3_stmt *statement;
        int result = sqlite3_prepare_v2(handle, query.toUtf8(), -1, &statement, NULL);
        if(isError(result))
            throwException(result);
        return statement;
    }

    static bool step(sqlite3_stmt *statement)
    {
        int result = sqlite3_step(statement);
        if(result == SQLITE_ROW)
            return true;
        else if(result == SQLITE_DONE)
            return false;
        else
            throwException(result);
        return false;
    }

    static void reset(sqlite3_stmt *statement)
    {
        int result = sqlite3_reset(statement);
        if(isError(result))
            throwException(result);
    }

    static void finalize(sqlite3_stmt *statement)
    {
        int result = sqlite3_finalize(statement);
        if(isError(result))
            throwException(result);
    }

    static QString getSql(sqlite3_stmt *statement)
    {
        return sqlite3_sql(statement);
    }

    static void bindInt(sqlite3_stmt *statement, int param, int value)
    {
        int result = sqlite3_bind_int(statement, param, value);
        if(isError(result))
            throwException(result);
    }

    static void bindDouble(sqlite3_stmt *statement, int param, double value)
    {
        int result = sqlite3_bind_double(statement, param, value);
        if(isError(result))
            throwException(result);
    }

    static void bindText(sqlite3_stmt *statement, int param, const QString &value)
    {
        int result = sqlite3_bind_text(statement, param, value.toUtf8(), -1, NULL);
        if(isError(result))
            throwException(result);
    }

    static int columnCount(sqlite3_stmt *statement)
    {
        return sqlite3_column_count(statement);
    }

    static QString columnName(sqlite3_stmt *statement, int column)
    {
        return sqlite3_column_name(statement, column);
    }

    static int columnInt(sqlite3_stmt *statement, int column)
    {
        return sqlite3_column_int(statement, column);
    }

    static double columnDouble(sqlite3_stmt *statement, int column)
    {
        return sqlite3_column_double(statement, column);
    }

    static QString columnText(sqlite3_stmt *statement, int column)
    {
        return reinterpret_cast<const char*>(sqlite3_column_text(statement, column));
    }

    static void progressHandler(sqlite3 *handle, int operationInterval, int (*function)(void *), void *data)
    {
        sqlite3_progress_handler(handle, operationInterval, function, data);
    }

    static QString getErrorString(int result)
    {
        return sqlite3_errstr(result);
    }

    static bool isError(int result)
    {
        return result != SQLITE_OK;
    }

private:
    static void throwException(int result)
    {
        throw Exception(getErrorString(result));
    }

};

} // namespace sqlite

#endif // SQLITEAPI_H
