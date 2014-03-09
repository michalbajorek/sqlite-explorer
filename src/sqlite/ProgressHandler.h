#ifndef PROGRESS_HANDLER_H
#define PROGRESS_HANDLER_H

#include <QObject>
#include <QSet>

#include "Object.h"

namespace sqlite
{

class Database;

class ProgressHandler : public QObject, public Object
{
    Q_OBJECT
public:
    ProgressHandler(Database *database);
    ~ProgressHandler();

    void setEnabled(bool newEnabled);
    bool isEnabled()
        { return enabled; }

    void setOperationInterval(int operationInterval);

signals:
    void progress(/*Database* database, bool &cancelOperation*/);

private:
    void setHandler();
    void removeHandler();
    bool emitSignal();

    static int staticProgressHandler(void *param);

    int operationInterval;
    bool enabled;

};

} // namespace sqlite

#endif // PROGRESS_HANDLER_H
