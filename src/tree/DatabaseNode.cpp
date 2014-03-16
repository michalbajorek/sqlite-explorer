#include <QFileInfo>
#include <QIcon>

#include "DatabaseNode.h"
#include "TableNode.h"

using namespace tree;

DatabaseNode::DatabaseNode(sqlite::Database *database)
{
    this->database = database;
}

QString DatabaseNode::getText() const
{
    QFileInfo fileInfo(database->getFileName());
    return fileInfo.fileName();
}

QIcon DatabaseNode::getIcon() const
{
    return QIcon(":/Icons/16/databaseIcon");
}

sqlite::Table *DatabaseNode::getTable() const
{
    return database->tables.getMasterTable();
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
