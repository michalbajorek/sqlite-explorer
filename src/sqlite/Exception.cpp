#include "exception.h"
#include "sqlite3.h"

using namespace sqlite;

#ifdef _DEBUG
Exception::Exception(int errorCode, const char *sourceFileName, int lineNumber, const char *functionName)
{
    this->errorCode = errorCode;
    this->sourceFileName = sourceFileName;
    this->lineNumber = lineNumber;
    this->functionName = functionName;
}
#else
Exception::Exception(int errorCode)
{
    this->errorCode = errorCode;
}
#endif

Exception::Exception(const QString &errorMessage)
{
    errorCode = NoErrorCode;
    this->errorMessage = errorMessage;
}

QString Exception::getErrorMessage()
{
    if(errorCode != NoErrorCode)
    {
    #ifdef _DEBUG
        errorMessage = "Message: ";
        errorMessage += sqlite3_errstr(errorCode);
        errorMessage += "\n";
        errorMessage += "Source: ";
        errorMessage += sourceFileName;
        errorMessage += " [";
        errorMessage += QString::number(lineNumber);
        errorMessage += "]\n";
        errorMessage += "Function: ";
        errorMessage += functionName;
    #else
        errorMessage = sqlite3_errstr(errorCode);
    #endif
    }
    return errorMessage;
}

#ifdef _DEBUG
void sqlite::checkErrorCodeAndThrowException_DEBUG(int errorCode, const char *fileName, int lineNumber, const char *functionName)
{
    if(errorCode != SQLITE_OK)
        throw Exception(errorCode, fileName, lineNumber, functionName);
}
#else
void sqlite::checkErrorCodeAndThrowException(int errorCode)
{
    if(errorCode != SQLITE_OK)
        throw Exception(errorCode);
}
#endif
