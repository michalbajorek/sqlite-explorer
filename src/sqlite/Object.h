#ifndef OBJECT_H
#define OBJECT_H

namespace sqlite
{

class Database;

class Object
{
public:
    Object(Database *database);
    virtual ~Object();

protected:
    Database *database;
};

} // namespace sqlite

#endif // OBJECT_H
