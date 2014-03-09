#ifndef TESTQUERY_H
#define TESTQUERY_H

#ifdef TESTCASE

#include <QtTest>
#include <QObject>

class Test : public QObject
{
    Q_OBJECT
private slots:
    // class Database
    void openNonexistentDatabase();
    void openExistingDatabase();
    void openDatabaseTwice();
    void getHandleNotOpenedDatabase();
    void getHandleOpenedDatabasde();
    void execVacuum();
    void executeProperSimpleQuery();
    void executeBadSimpleQuery();

    // class Query
    void executeProperQuery();
    void executeBadQuery();

};

#endif // TESTCASE

#endif // TESTQUERY_H
