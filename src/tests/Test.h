#ifndef TESTQUERY_H
#define TESTQUERY_H

#ifdef TESTCASE

#include <QtTest>
#include <QObject>

class Test : public QObject
{
    Q_OBJECT
private slots:
    // class sqlite::Database
    void openNonexistentDatabase();
    void openExistingDatabase();
    void openDatabaseTwice();
    void getHandleNotOpenedDatabase();
    void getHandleOpenedDatabasde();
    void execVacuum();
    void executeProperSimpleQuery();
    void executeBadSimpleQuery();

    // class sqlite::Query
    void executeProperQuery();
    void executeBadQuery();

    // class tree::Node
    void createDeleteSimpleTree();

};

#endif // TESTCASE

#endif // TESTQUERY_H
