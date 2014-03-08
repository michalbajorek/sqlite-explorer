#ifndef PROGRESS_HANDLER_H
#define PROGRESS_HANDLER_H

#include <QSet>

#include "object.h"

namespace sqlite
{

class Database;
class ProgressHandler;

class ProgressObserver
{
public:
    ProgressObserver();
    virtual ~ProgressObserver();

    virtual void onProgressHandler(Database *database, bool &cancelOperation) = 0;

private:
    ProgressHandler *progressHandler;

    friend class ProgressHandler;
};

class ProgressHandler : public Object
{
public:
    ProgressHandler(Database *database);
    ~ProgressHandler();

    void addObserver(ProgressObserver *observer);
    void removeObserver(ProgressObserver *observer);
    void setOperationInterval(int operationInterval);

private:
    void setHandler();
    void removeHandler();
    bool notifyObservers();
    void removeAllObservers();

    static int staticProgressHandler(void *param);

    int operationInterval;
    QSet<ProgressObserver*> observersSet;

};

} // namespace sqlite

#endif // PROGRESS_HANDLER_H
