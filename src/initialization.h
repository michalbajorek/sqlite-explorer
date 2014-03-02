#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include <QTextCodec>

class Initialization
{
public:
    Initialization();

    void setUtf8EncodingForCStrings()
    {
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    }
};

#endif // INITIALIZATION_H
