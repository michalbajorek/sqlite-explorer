#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>

namespace sqlite
{

class Exception
{
public:
    Exception(int errorCode);
    Exception(const QString &errorMessage);

    int getErrorCode() const
        { return errorCode; }

    QString getErrorMessage();

    static const int NoErrorCode = -1;

private:
    int errorCode;
    QString errorMessage;


};

void checkErrorCodeAndThrowException(int errorCode);

} // namespace sqlite

#endif // EXCEPTION_H
