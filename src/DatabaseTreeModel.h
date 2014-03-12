#ifndef DATABASETREEMODEL_H
#define DATABASETREEMODEL_H

#include <QAbstractItemModel>
#include <QList>

#include "DatabaseTree.h"
#include "sqlite/Database.h"

class DatabaseTreeModel : public QAbstractItemModel
{
public:
    DatabaseTreeModel();
    virtual ~DatabaseTreeModel();

    void addDatabase(sqlite::Database *database);
    void removeDatabase(sqlite::Database *database);

private:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;

    void addDatabaseNode(sqlite::Database *database);
    void removeDatabaseNode(sqlite::Database *database);
    DatabaseNode *createDatabaseTree(sqlite::Database *database);
    DatabaseNode *findDatabaseNode(sqlite::Database *database);

    QList<DatabaseNode*> databaseNodeList;
};

#endif // DATABASETREEMODEL_H
