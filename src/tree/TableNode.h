#ifndef TABLENODE_H
#define TABLENODE_H

#include "Node.h"
#include "../sqlite/Table.h"

namespace tree
{

class TableNode : public Node
{
public:
    TableNode(sqlite::Table *table);

    virtual QString getText(int column) const;
    virtual QIcon getIcon() const;

    sqlite::Table* getTable() const
        { return table; }

private:
    sqlite::Table *table;

};

} // namespace tree

#endif // TABLENODE_H
