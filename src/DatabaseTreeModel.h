#ifndef DATABASETREEMODEL_H
#define DATABASETREEMODEL_H

#include <QAbstractItemModel>
#include <QHash>
#include <QIcon>
#include <QList>
#include <QString>

#include "tree/DatabaseNode.h"
#include "tree/Node.h"
#include "sqlite/Database.h"

typedef QHash<QString, sqlite::Database*> DatabaseHash;

class DatabaseTreeModel : public QAbstractItemModel
{
public:
    DatabaseTreeModel();
    virtual ~DatabaseTreeModel();

    void setDatabaseList(QList<sqlite::Database*> &databaseList);

private:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void addDatabaseNode(sqlite::Database *database);
    void removeDatabaseNode(sqlite::Database *database);
    tree::DatabaseNode* createDatabaseTree(sqlite::Database *database);
    tree::DatabaseNode* findDatabaseNode(sqlite::Database *database);
    tree::ViewNode* getViewNodeFromIndex(const QModelIndex &index) const;
    void createDatabaseNodeList(QList<sqlite::Database*> &databaseList);
    void deleteDatabaseNodeList();
    QString getDisplayRole(const QModelIndex &index) const;
    QIcon getDecorationRole(const QModelIndex &index) const;

    QList<tree::DatabaseNode*> databaseNodeList;
};

#endif // DATABASETREEMODEL_H
