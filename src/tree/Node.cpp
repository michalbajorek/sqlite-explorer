#include "Node.h"
#include "../common/Exception.h"

using namespace tree;

Node::Node()
{
    parent = nullptr;
    index = 0;
}

Node::~Node()
{
    deleteChildren();
}

void Node::deleteChildren()
{
    foreach(Node *child, children)
        delete child;
    children.clear();
}

void Node::addChild(Node *childNode)
{
    if(childNode->getParent() != nullptr)
        throw common::Exception("Child node has alredy parent");
    children.append(childNode);
    childNode->setParent(this);
}

void Node::setParent(Node *newParent)
{
    parent = newParent;
    index = parent->getChildrenCount() - 1;
}

QString ViewNode::getRecordsCountText() const
{
    sqlite::Table *table = getTable();
    if(table->isLoaded())
        return QString::number(table->getRecordsCount());
    else
        return QString();
}
