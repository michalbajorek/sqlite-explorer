#ifndef PROGRESS_HANDLER_H
#define PROGRESS_HANDLER_H

#include "object.h"

namespace sqlite
{

class Database;

class ProgressHandler : public Object
{
    typedef void (*Function)(Database *database, bool &cancelOperation);

public:
    ProgressHandler(Database *database);

    void set(Function function, int operationInterval = 10);
    void remove();

private:
    static int staticProgressHandler(void *param);
    bool invokeFunction();

    Function function;


};

} // namespace sqlite

#endif // PROGRESS_HANDLER_H
