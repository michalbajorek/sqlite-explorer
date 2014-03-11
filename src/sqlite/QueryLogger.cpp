#include <QDebug>

#include "QueryLogger.h"

using namespace sqlite;

void sqlite::ConsoleQueryLogger::logQuery(const QString &query)
{
    qDebug() << "Query log: " << query;
}
