#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>

namespace sqlite
{

class Exception
{
public:
    #ifdef _DEBUG
    Exception(int errorCode, const char *sourceFileName, int lineNumber, const char *functionName);
    #else
    Exception(int errorCode);
    #endif
    Exception(const QString &errorMessage);

    int getErrorCode() const
        { return errorCode; }

    QString getErrorMessage();

    static const int NoErrorCode = -1;

private:
    int errorCode;
    QString errorMessage;

    #ifdef _DEBUG
    const char *sourceFileName;
    int lineNumber;
    const char *functionName;
    #endif
};


#ifdef _DEBUG
#define checkErrorCodeAndThrowException(errorCode) \
        checkErrorCodeAndThrowException_DEBUG(errorCode, __FILE__, __LINE__, __FUNCTION__)

void checkErrorCodeAndThrowException_DEBUG(int errorCode, const char *fileName, int lineNumber, const char *functionName);
#else
void checkErrorCodeAndThrowException(int errorCode);
#endif
} // namespace sqlite

#endif // EXCEPTION_H
