#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include <QTextCodec>
#include <QtGlobal>

class Initialization
{
public:
    Initialization();

    void setUtf8EncodingForCStrings()
    {
        #if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
            QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        #endif
    }
};

#endif // INITIALIZATION_H
