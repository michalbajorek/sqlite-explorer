#ifndef QUERY_H
#define QUERY_H

#include <QString>

#include "object.h"
#include "sqlite3.h"


namespace sqlite
{

class Database;

class Query : public Object
{
public:
    Query(Database *database);
    Query(Database *database, const QString &queryText);

    virtual ~Query();

    void prepare(const QString &queryText);
    bool step();
    void reset();
    void finalize();

    bool isDone()
        { return lastStepResult == SQLITE_DONE; }

    int getColumnsCount()
        { return sqlite3_column_count(statement); }

    QString getColumnName(int columnIndex)
        { return sqlite3_column_name(statement, columnIndex); }

    QString getQueryText()
        { return sqlite3_sql(statement); }

    int getIntegerValue(int columnIndex)
        { return sqlite3_column_int(statement, columnIndex); }

    int64_t getInt64Value(int columnIndex)
        { return sqlite3_column_int64(statement, columnIndex); }

    double getDoubleValue(int columnIndex)
        { return sqlite3_column_double(statement, columnIndex); }

    QString getTextValue(int columnIndex)
        { return reinterpret_cast<const char*>(sqlite3_column_text(statement, columnIndex)); }

    void bindInteger(int paramIndex, int value);
    void bindInt64(int paramIndex, int64_t value);
    void bindDouble(int paramIndex, double value);
    void bindText(int paramIndex, const QString &value);

private:
    void init();

    sqlite3_stmt *statement;
    int lastStepResult;
};

} // namespace sqlite

#endif // QUERY_H
