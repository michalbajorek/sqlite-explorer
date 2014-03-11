#ifndef QUERY_H
#define QUERY_H

#include <QString>

#include "Api.h"
#include "Object.h"

namespace sqlite
{

class Database;

class Query : public Object
{
public:
    Query(Database *database);

    virtual ~Query();

    void prepare(const QString &query);
    bool step();
    void reset();
    void finalize();

    bool isActive() const
        { return statement != NULL; }

    bool isDone() const // TODO: Function name isDone is misleading
        { return !isInProgress; }

    int getColumnsCount() const
        { return Api::columnCount(statement); }

    QString getColumnName(int column) const
        { return Api::columnName(statement, column); }

    QString getQueryText() const
        { return Api::getSql(statement); }

    int getIntegerValue(int column) const
        { return Api::columnInt(statement, column); }

    double getDoubleValue(int column) const
        { return Api::columnDouble(statement, column); }

    QString getTextValue(int column) const
        { return Api::columnText(statement, column); }

    void bindInteger(int param, int value)
        { Api::bindInt(statement, param, value); }

    void bindDouble(int param, double value)
        { Api::bindDouble(statement, param, value); }

    void bindText(int param, const QString &value)
        { Api::bindText(statement, param, value); }

private:
    void init();

    sqlite3_stmt *statement;
    bool isInProgress;
};

} // namespace sqlite

#endif // QUERY_H
