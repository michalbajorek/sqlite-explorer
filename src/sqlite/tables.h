#ifndef TABLES_H
#define TABLES_H

namespace sqlite
{

class Tables : public QList<Table>, public Object
{
public:
    Tables(Database *database);

private:
};

}// namespace sqlite

#endif // TABLES_H
