#ifndef DATABASENODE_H
#define DATABASENODE_H

#include <QString>

#include "TableNode.h"
#include "../sqlite/Database.h"
#include "../sqlite/Table.h"

namespace tree
{

class DatabaseNode : public TableNode
{
public:
    DatabaseNode(sqlite::Database *database);

    virtual QString getText(int column) const;
    virtual QIcon getIcon() const;

    sqlite::Database* getDatabase() const
        { return database; }

    void createTableNodes();

private:
    void addTableNode(sqlite::Table *table);

    sqlite::Database *database;
};

} // namespace tree

#endif // DATABASENODE_H
