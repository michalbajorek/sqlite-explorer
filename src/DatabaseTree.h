#ifndef DATABASETREE_H
#define DATABASETREE_H

#include "sqlite/Database.h"
#include "sqlite/Object.h"
#include "sqlite/Table.h"

class TreeNode
{
public:
    TreeNode(void);
    virtual ~TreeNode();

    void addChild(TreeNode *child);
    void deleteChildren();
    TreeNode* getChild(int row) const;
    int getChildCount() const;
    int getParentIndex() const;
    TreeNode *getParent() const;
    virtual QString getText(int column) const;

private:
    void setParent(TreeNode *newParent);

    TreeNode *parent;
    QList<TreeNode*> children;
    int parentIndex;
};


class DatabaseNode : public TreeNode
{
public:
    DatabaseNode(sqlite::Database *database)
        { this->database = database; }

    virtual QString getText(int column) const;

    sqlite::Database *getDatabase()
        { return database; }

    sqlite::Table *getMasterTable()
        { return database->tables.getMasterTable(); }

    void createChildren();

private:
    void addTableNode(sqlite::Table *table);

    sqlite::Database *database;
};


class TableNode : public TreeNode
{
public:
    TableNode(sqlite::Table *table)
        { this->table = table; }

    virtual QString getText(int column) const;

    sqlite::Table *getTable()
        { return table; }

private:
    sqlite::Table *table;
};

#endif // DATABASETREE_H
