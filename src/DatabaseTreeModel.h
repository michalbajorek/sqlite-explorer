#ifndef DATABASETREEMODEL_H
#define DATABASETREEMODEL_H

#include <QAbstractItemModel>

#include "sqlite/Database.h"
#include "sqlite/Object.h"
#include "sqlite/Table.h"

class TreeNode
{
public:
     TreeNode(void)
     {
        parent = NULL;
        row = -1;
     }

     virtual ~TreeNode()
     {
        deleteChildren();
     }

     void deleteChildren()
     {
         foreach(TreeNode *child, children)
            delete child;
         children.clear();
     }

     void addChild(TreeNode *child)
     {
        children.append(child);
        child->row = children.count() - 1;
        child->parent = this;
     }

     TreeNode* getChild(int row) const
     {
        return children.at(row);
     }

     int getChildCount() const
     {
        return children.count();
     }

     int getRow() const
     {
        return row;
     }

     TreeNode *getParent() const
     {
        return parent;
     }

     virtual QString getText()
        { return QString(); }

private:
    int row;
    TreeNode *parent;
    QList<TreeNode*> children;
};


class DatabaseNode : public TreeNode
{
public:
    DatabaseNode(sqlite::Database *database)
    {
        this->database = database;
    }

    virtual QString getText()
    {
        return database->getFileName();
    }
    sqlite::Database *getDatabase()
    {
        return database;
    }

    sqlite::Table *getMasterTable()
    {
        return database->tables.getMasterTable();
    }

private:
    sqlite::Database *database;
};


class TableNode : public TreeNode
{
public:
    TableNode(sqlite::Table *table)
    {
        this->table = table;
    }

    virtual QString getText()
    {
        return table->getName();
    }

    sqlite::Table *getTable()
    {
        return table;
    }

private:
    sqlite::Table *table;
};



class DatabaseTreeModel : public QAbstractItemModel
{
public:
    DatabaseTreeModel();

    void setDatabase(sqlite::Database *newDatabase);
    void clearDatabase()
        { setDatabase(NULL); }

private:
    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;

    void createTree();
    void deleteTree();

    sqlite::Database *database;
    TreeNode rootNode;

    // QAbstractItemModel interface
public:
//    virtual bool hasChildren(const QModelIndex &parent) const;
};

#endif // DATABASETREEMODEL_H
