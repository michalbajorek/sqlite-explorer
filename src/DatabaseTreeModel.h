#ifndef DATABASETREEMODEL_H
#define DATABASETREEMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QHash>

#include "DatabaseTree.h"
#include "sqlite/Database.h"

typedef QHash<QString, sqlite::Database*> DatabaseHash;

class DatabaseTreeModel : public QAbstractItemModel
{
public:
    DatabaseTreeModel();
    virtual ~DatabaseTreeModel();

    void setDatabaseHash(DatabaseHash &databaseHash);

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
    void createDatabaseNodeList(DatabaseHash &databaseHash);
    void deleteDatabaseNodeList();


    QList<DatabaseNode*> databaseNodeList;
};

#endif // DATABASETREEMODEL_H
