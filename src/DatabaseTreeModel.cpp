#include "DatabaseTreeModel.h"


DatabaseTreeModel::DatabaseTreeModel()
{
}

DatabaseTreeModel::~DatabaseTreeModel()
{

}

void DatabaseTreeModel::setDatabaseHash(DatabaseHash &databaseHash)
{
    beginResetModel();
    deleteDatabaseNodeList();
    createDatabaseNodeList(databaseHash);
    endResetModel();
}

void DatabaseTreeModel::deleteDatabaseNodeList()
{
    foreach(DatabaseNode *node, databaseNodeList)
        delete node;
    databaseNodeList.clear();
}

void DatabaseTreeModel::createDatabaseNodeList(DatabaseHash &databaseHash)
{
    foreach(sqlite::Database *database, databaseHash)
        addDatabaseNode(database);
}

void DatabaseTreeModel::addDatabaseNode(sqlite::Database *database)
{
    DatabaseNode *databaseRoot = createDatabaseTree(database);
    databaseNodeList.append(databaseRoot);
}

DatabaseNode* DatabaseTreeModel::createDatabaseTree(sqlite::Database *database)
{
    DatabaseNode *databaseNode = new DatabaseNode(database);
    databaseNode->createChildren();
    return databaseNode;
}

void DatabaseTreeModel::removeDatabaseNode(sqlite::Database *database)
{
    DatabaseNode *databaseNode = findDatabaseNode(database);
    delete databaseNode;
}

DatabaseNode *DatabaseTreeModel::findDatabaseNode(sqlite::Database *database)
{
    foreach(DatabaseNode *node, databaseNodeList)
        if(node->getDatabase() == database)
            return node;
    throw sqlite::Exception("Database not found in tree");
}

QModelIndex DatabaseTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        TreeNode *parentNode = (TreeNode*)parent.internalPointer();
        return createIndex(row, column, parentNode->getChild(row));
    }
    else if(row < databaseNodeList.count())
    {
        TreeNode *childNode = databaseNodeList.at(row);
        return createIndex(row, column, childNode);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex DatabaseTreeModel::parent(const QModelIndex &child) const
{
    if(child.isValid())
    {
        TreeNode *childNode = (TreeNode*)child.internalPointer();
        TreeNode *parentNode = childNode->getParent();
        if(parentNode)
            return createIndex(parentNode->getParentIndex(), 0, parentNode);
        else
            return QModelIndex();
    }
    else
    {
        return QModelIndex();
    }
}

int DatabaseTreeModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        TreeNode *parentNode = (TreeNode*)parent.internalPointer();
        if(parentNode)
            return parentNode->getChildCount();
        else
            return 0;
    }
    else
    {
        return databaseNodeList.count();
    }

}

int DatabaseTreeModel::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant DatabaseTreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    switch(role)
    {
    case Qt::DisplayRole:
        {
        TreeNode *node = static_cast<TreeNode*>(index.internalPointer());
        return node->getText(index.column());
        }
    default:
        break;
    }
    return QVariant();
}
