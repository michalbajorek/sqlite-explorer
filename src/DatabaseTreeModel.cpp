#include "DatabaseTreeModel.h"


DatabaseTreeModel::DatabaseTreeModel()
{
}

DatabaseTreeModel::~DatabaseTreeModel()
{

}

void DatabaseTreeModel::setDatabaseList(QList<sqlite::Database*> &databaseList)
{
    beginResetModel();
    deleteDatabaseNodeList();
    createDatabaseNodeList(databaseList);
    endResetModel();
}

void DatabaseTreeModel::deleteDatabaseNodeList()
{
    foreach(tree::DatabaseNode *node, databaseNodeList)
        delete node;
    databaseNodeList.clear();
}

void DatabaseTreeModel::createDatabaseNodeList(QList<sqlite::Database*> &databaseList)
{
    foreach(sqlite::Database *database, databaseList)
        addDatabaseNode(database);
}

void DatabaseTreeModel::addDatabaseNode(sqlite::Database *database)
{
    tree::DatabaseNode *databaseRoot = createDatabaseTree(database);
    databaseNodeList.append(databaseRoot);
}

tree::DatabaseNode* DatabaseTreeModel::createDatabaseTree(sqlite::Database *database)
{
    tree::DatabaseNode *node = new tree::DatabaseNode(database);
    node->createTableNodes();
    return node;
}

void DatabaseTreeModel::removeDatabaseNode(sqlite::Database *database)
{
    tree::DatabaseNode *node = findDatabaseNode(database);
    delete node;
}

tree::DatabaseNode *DatabaseTreeModel::findDatabaseNode(sqlite::Database *database)
{
    foreach(tree::DatabaseNode *node, databaseNodeList)
        if(node->getDatabase() == database)
            return node;
    throw common::Exception("Database not found in tree");
}

QModelIndex DatabaseTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        tree::Node *parentNode = (tree::Node*)parent.internalPointer();
        return createIndex(row, column, parentNode->getChild(row));
    }
    else if(row < databaseNodeList.count())
    {
        tree::Node *childNode = databaseNodeList.at(row);
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
        tree::Node *childNode = static_cast<tree::Node*>(child.internalPointer());
        tree::Node *parentNode = childNode->getParent();
        if(parentNode)
            return createIndex(parentNode->getIndex(), 0, parentNode);
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
        tree::Node *parentNode = static_cast<tree::Node*>(parent.internalPointer());
        if(parentNode)
            return parentNode->getChildrenCount();
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
            return getDisplayRole(index);
        case Qt::DecorationRole:
            if(index.column() == 0)
                return getDecorationRole(index);
            break;
        default:
            break;
    }
    return QVariant();
}

tree::TableNode* DatabaseTreeModel::getTableNodeFromIndex(const QModelIndex &index) const
{
    return static_cast<tree::TableNode*>(index.internalPointer());
}

QString DatabaseTreeModel::getDisplayRole(const QModelIndex &index) const
{
    tree::TableNode *node = getTableNodeFromIndex(index);
    return node->getText(index.column());
}

QIcon DatabaseTreeModel::getDecorationRole(const QModelIndex &index) const
{
    tree::TableNode *node = getTableNodeFromIndex(index);
    return node->getIcon();
}

QVariant DatabaseTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        switch(role)
        {
            case Qt::DisplayRole:
                if(section == 0)
                    return QVariant("Name");
                else
                    return QVariant("Record count");
            default:
                return QVariant();
        }
    }
    else
        return QVariant();
}
