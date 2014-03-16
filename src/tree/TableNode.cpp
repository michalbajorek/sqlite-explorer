#include "TableNode.h"

using namespace tree;

TableNode::TableNode(sqlite::Table *table)
{
    this->table = table;
}

QString tree::TableNode::getText() const
{
    return table->getName();
}

QIcon TableNode::getIcon() const
{
    return QIcon(":/Icons/16/tableIcon");
}

sqlite::Table *TableNode::getTable() const
{
    return table;
}
