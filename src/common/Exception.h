#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>

namespace common
{

class Exception
{
public:
    Exception(const QString &errorMessage)
        { this->errorMessage = errorMessage; }

    const QString &getErrorMessage()
        { return errorMessage; }

private:
    QString errorMessage;
};

} // namespace common

#endif // EXCEPTION_H
