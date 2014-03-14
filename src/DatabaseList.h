#ifndef DATABASELIST_H
#define DATABASELIST_H

#include <QHash>
#include <QString>

#include "DatabaseTreeModel.h"
#include "sqlite/Database.h"


class DatabaseList
{
public:
    DatabaseList();
    ~DatabaseList();

    void addDatabase(const QString &fileName);
    void removeDatabase(sqlite::Database *database);
    void beginUpdate();
    void endUpdate();

    QAbstractItemModel *getModel()
        { return &model; }

    void removeAllDatabases();
private:
    void updateModel();

    QList<sqlite::Database*> databaseList;
    DatabaseTreeModel model;
    bool isUpdating;
};

#endif // DATABASELIST_H
