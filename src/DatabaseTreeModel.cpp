#include "DatabaseTreeModel.h"


DatabaseTreeModel::DatabaseTreeModel()
{
    database = NULL;
}

void DatabaseTreeModel::setDatabase(sqlite::Database *newDatabase)
{
    beginResetModel();
    database = newDatabase;
    deleteTree();
    if(database)
        createTree();
    endResetModel();
}

QModelIndex DatabaseTreeModel::index(int row, int column, const QModelIndex &parent) const
{
//     if (!hasIndex(row, column, parent))
//         return QModelIndex();

     const TreeNode *parentItem;

     if (!parent.isValid())
         parentItem = &rootNode;
     else
         parentItem = static_cast<TreeNode*>(parent.internalPointer());
    if(parentItem == NULL)
        return QModelIndex();

     TreeNode *childItem = parentItem->getChild(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
}

QModelIndex DatabaseTreeModel::parent(const QModelIndex &child) const
{
     if (!child.isValid())
         return QModelIndex();

     TreeNode *childItem = static_cast<TreeNode*>(child.internalPointer());
     TreeNode *parentItem = childItem->getParent();

     return createIndex(parentItem ? parentItem->getRow() : 0, 0, parentItem);
}

int DatabaseTreeModel::rowCount(const QModelIndex &parent) const
{
     const TreeNode *parentItem;

     if (!parent.isValid())
         parentItem = &rootNode;
     else
         parentItem = (TreeNode*)parent.internalPointer();
    if(parentItem)
        return parentItem->getChildCount();
    return 0;
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

void DatabaseTreeModel::createTree()
{
    DatabaseNode *databaseNode = new DatabaseNode(database);
    rootNode.addChild(databaseNode);
    for(int i = 0; i < database->tables.getCount(); i++)
    {
        sqlite::Table *table = database->tables.getTable(i);
        TableNode *tableNode = new TableNode(table);
        databaseNode->addChild(tableNode);
    }
}

void DatabaseTreeModel::deleteTree()
{
    rootNode.deleteChildren();
}

//bool DatabaseTreeModel::hasChildren(const QModelIndex &parent) const
//{
//    if(parent.isValid())
//    {
//        TreeNode *node = (TreeNode*)parent.internalPointer();
//        if(node)
//            return node->getChildCount();
//    }
//    return false;
//}
