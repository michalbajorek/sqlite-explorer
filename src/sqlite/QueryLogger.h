#ifndef QUERYLOGGER_H
#define QUERYLOGGER_H

#include <QStringList>

namespace sqlite
{

class QueryLogger
{
public:
    virtual void logQuery(const QString &query) = 0;
};

class ConsoleQueryLogger : public QueryLogger
{
public:
    virtual void logQuery(const QString &query);
};

} // namespace sqlite

#endif // QUERYLOGGER_H
