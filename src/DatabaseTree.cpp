#include <QFileInfo>
#include "DatabaseTree.h"


TreeNode::TreeNode()
{
    parent = NULL;
    parentIndex = 0;
}

TreeNode::~TreeNode()
{
    deleteChildren();
}

void TreeNode::deleteChildren()
{
    foreach(TreeNode *child, children)
        delete child;
    children.clear();
}

void TreeNode::addChild(TreeNode *child)
{
    children.append(child);
    child->setParent(this);
}

void TreeNode::setParent(TreeNode *newParent)
{
    parent = newParent;
    parentIndex = newParent->children.count() - 1;
}

QString TreeNode::getText() const
{
    return QString();
}

TreeNode* TreeNode::getChild(int row) const
{
    return children.at(row);
}

int TreeNode::getChildCount() const
{
    return children.count();
}

int TreeNode::getParentIndex() const
{
    return parentIndex;
}

TreeNode *TreeNode::getParent() const
{
    return parent;
}



void DatabaseNode::addTableNode(sqlite::Table *table)
{
    TableNode *tableNode = new TableNode(table);
    addChild(tableNode);
}

QString DatabaseNode::getText() const
{
    QFileInfo fileInfo(database->getFileName());
    return fileInfo.fileName();
}

void DatabaseNode::createChildren()
{
    for(int i = 0; i < database->tables.getCount(); i++)
        addTableNode(database->tables.getTable(i));
}
