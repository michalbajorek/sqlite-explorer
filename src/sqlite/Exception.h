#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>

namespace sqlite
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

} // namespace sqlite

#endif // EXCEPTION_H
