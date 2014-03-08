#include "Exception.h"
#include "sqlite3.h"

using namespace sqlite;


Exception::Exception(int errorCode)
{
    this->errorCode = errorCode;
}

Exception::Exception(const QString &errorMessage)
{
    errorCode = NoErrorCode;
    this->errorMessage = errorMessage;
}

QString Exception::getErrorMessage()
{
    if(errorCode != NoErrorCode)
        errorMessage = sqlite3_errstr(errorCode);
    return errorMessage;
}

void sqlite::checkErrorCodeAndThrowException(int errorCode)
{
    if(errorCode != SQLITE_OK)
        throw Exception(errorCode);
}

