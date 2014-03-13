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
    void removeDatabase(const QString &fileName);
    bool isDatabaseOpened(const QString &fileName);
    void beginUpdate();
    void endUpdate();

    QAbstractItemModel *getModel()
        { return &model; }

private:
    sqlite::Database *createAndOpenDatabase(const QString &fileName);
    void closeAndDeleteDatabase(const QString &fileName);
    void updateModel();

    DatabaseHash databaseHash;
    DatabaseTreeModel model;
    bool isUpdating;
};

#endif // DATABASELIST_H
