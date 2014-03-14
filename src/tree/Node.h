#ifndef NODE_H
#define NODE_H

#include <QList>
#include <QString>

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

} // namespace tree

#endif // NODE_H
