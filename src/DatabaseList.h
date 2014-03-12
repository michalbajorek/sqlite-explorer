#ifndef DATABASELIST_H
#define DATABASELIST_H

#include <QList>
#include <QString>

#include "DatabaseTreeModel.h"
#include "sqlite/Database.h"

class DatabaseList
{
public:
    DatabaseList();
    ~DatabaseList();

    void addAndOpenDatabase(const QString &fileName);

    QAbstractItemModel *getModel()
        { return &model; }

private:
    QList<sqlite::Database*> databaseList;
    DatabaseTreeModel model;
};

#endif // DATABASELIST_H
