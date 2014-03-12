#include "DatabaseTreeModel.h"


DatabaseTreeModel::DatabaseTreeModel()
{
}

DatabaseTreeModel::~DatabaseTreeModel()
{

}

void DatabaseTreeModel::addDatabase(sqlite::Database *database)
{
    beginResetModel();
    addDatabaseNode(database);
    endResetModel();
}

void DatabaseTreeModel::addDatabaseNode(sqlite::Database *database)
{
    DatabaseNode *databaseNode = createDatabaseTree(database);
    databaseNodeList.append(databaseNode);
}

DatabaseNode* DatabaseTreeModel::createDatabaseTree(sqlite::Database *database)
{
    DatabaseNode *databaseNode = new DatabaseNode(database);
    databaseNode->createChildren();
    return databaseNode;
}

void DatabaseTreeModel::removeDatabase(sqlite::Database *database)
{
    beginResetModel();
    removeDatabaseNode(database);
    endResetModel();
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
    return 1;
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
        return node->getText();
        }
    default:
        break;
    }
    return QVariant();
}
