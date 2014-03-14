#ifndef DATABASEREPOSITORY_H
#define DATABASEREPOSITORY_H

#include <QHash>
#include <QString>

#include "Database.h"

namespace sqlite
{

class DatabaseRepository
{
public:
    static Database *get(const QString &fileName);
    static void release(Database *database);
    static bool isOpened(const QString &fileName);

private:
    static Database* createAndOpenDatabase(const QString &fileName);
    static QHash<QString, Database*> databaseHash;
};

} // namespace sqlite

#endif // DATABASEREPOSITORY_H
