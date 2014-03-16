#ifndef DATABASENODE_H
#define DATABASENODE_H

#include <QIcon>
#include <QString>

#include "Node.h"
#include "../sqlite/Database.h"
#include "../sqlite/Table.h"

namespace tree
{

class DatabaseNode : public ViewNode
{
public:
    DatabaseNode(sqlite::Database *database);

    virtual QString getText() const;
    virtual QIcon getIcon() const;
    virtual sqlite::Table* getTable() const;

    sqlite::Database* getDatabase() const
        { return database; }

    void createTableNodes();

private:
    void addTableNode(sqlite::Table *table);

    sqlite::Database *database;
};

} // namespace tree

#endif // DATABASENODE_H
