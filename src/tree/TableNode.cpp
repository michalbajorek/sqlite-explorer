#include <QIcon>
#include "TableNode.h"

using namespace tree;

TableNode::TableNode(sqlite::Table *table)
{
    this->table = table;
}

QString tree::TableNode::getText(int column) const
{
    if(column == 0)
        return table->getName();
    else
    {
        if(table->isLoaded())
            return QString::number(table->getRecordsCount());
        else
            return QString();
    }
}

QIcon TableNode::getIcon() const
{
    return QIcon(":/Icons/16/tableIcon");
}
