#ifndef TABLENODE_H
#define TABLENODE_H

#include <QIcon>
#include <QString>

#include "Node.h"
#include "../sqlite/Table.h"

namespace tree
{

class TableNode : public ViewNode
{
public:
    TableNode(sqlite::Table *table);

    virtual QString getText() const;
    virtual QIcon getIcon() const;
    virtual sqlite::Table* getTable() const;

private:
    sqlite::Table *table;

};

} // namespace tree

#endif // TABLENODE_H
