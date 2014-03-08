#ifndef TESTQUERY_H
#define TESTQUERY_H

#ifdef TESTCASE

#include <QtTest/QtTest>
#include <QObject>
#include <QDebug>

#include "../sqlite/Database.h"

class Test : public QObject
{
    Q_OBJECT
private slots:
    void openNonexistentDatabase();
    void openExistingDatabase();
    void openDatabaseTwice();
    void getHandleNotOpenedDatabase();
    void getHandleOpenedDatabasde();

private:

};

#endif // TESTCASE

#endif // TESTQUERY_H
