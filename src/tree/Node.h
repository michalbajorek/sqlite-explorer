#ifndef NODE_H
#define NODE_H

#include <QIcon>
#include <QList>
#include <QString>

#include "../sqlite/Table.h"

namespace tree
{

class Node
{
public:
    Node();
    virtual ~Node();

    void addChild(Node *childNode);

    Node* getChild(int childIndex) const
        { return children.at(childIndex); }

    Node* getParent() const
        { return parent; }

    int getChildrenCount() const
        { return children.count(); }

    int getIndex() const
        { return index; }

private:
    void setParent(Node *newParent);
    void deleteChildren();

    Node *parent;
    QList<Node*> children;
    int index;

};

class ViewNode : public Node
{
public:
    virtual QString getText() const = 0;
    virtual QIcon getIcon() const = 0;
    virtual sqlite::Table* getTable() const  = 0;
    QString getRecordsCountText() const;
};

} // namespace tree

#endif // NODE_H
