#include <QFileInfo>
#include <QIcon>

#include "DatabaseNode.h"

using namespace tree;

DatabaseNode::DatabaseNode(sqlite::Database *database)
: TableNode(database->tables.getMasterTable())
{
    this->database = database;
}

QString DatabaseNode::getText(int column) const
{
    if(column == 0)
    {
        QFileInfo fileInfo(database->getFileName());
        return fileInfo.fileName();
    }
    else
        return QString();
}

QIcon DatabaseNode::getIcon() const
{
    return QIcon(":/Icons/16/databaseIcon");
}

void DatabaseNode::createTableNodes()
{
    int tableCount = database->tables.getCount();
    for(int i = 0; i < tableCount; i++)
        addTableNode(database->tables.getTable(i));
}

void DatabaseNode::addTableNode(sqlite::Table *table)
{
    TableNode *node = new TableNode(table);
    addChild(node);
}
