#include <QString>

#ifdef TESTCASE

#include "Test.h"
#include "TestThrowCheck.h"

#include "../sqlite/Database.h"
#include "../sqlite/Query.h"
#include "../tree/Node.h"

void Test::openNonexistentDatabase()
{
    sqlite::Database database;
    QVERIFY_THROW(database.open("FAKE DATABASE NAME"), sqlite::Exception);
}

void Test::openExistingDatabase()
{
    sqlite::Database database;
    QVERIFY_NOTHROW(database.open("testDatabase.sqlite"));
    QVERIFY_NOTHROW(database.close());
}

void Test::openDatabaseTwice()
{
    sqlite::Database database;
    QVERIFY_NOTHROW(database.open("testDatabase.sqlite"));
    QVERIFY_THROW(database.open("testDatabase.sqlite"), sqlite::Exception);
    QVERIFY_NOTHROW(database.close());
}

void Test::getHandleNotOpenedDatabase()
{
    sqlite::Database database;
    QVERIFY_THROW(database.getHandle(), sqlite::Exception);
    QVERIFY_THROW(database.close(), sqlite::Exception);
}

void Test::getHandleOpenedDatabasde()
{
    sqlite::Database database;
    QVERIFY_NOTHROW(database.open("testDatabase.sqlite"));
    QVERIFY_NOTHROW(database.getHandle());
    QVERIFY_NOTHROW(database.close());
}

void Test::execVacuum()
{
    sqlite::Database database;
    QVERIFY_NOTHROW(database.open("testDatabase.sqlite"));
    QVERIFY_NOTHROW(database.vacuum());
    QVERIFY_NOTHROW(database.close());
}

void Test::executeProperSimpleQuery()
{
    sqlite::Database database;
    QVERIFY_NOTHROW(database.open("testDatabase.sqlite"));
    QVERIFY_NOTHROW(database.executeSimpleQuery("BEGIN TRANSACTION"));
    QVERIFY_NOTHROW(database.executeSimpleQuery("END TRANSACTION"));
    QVERIFY_NOTHROW(database.close());
}

void Test::executeBadSimpleQuery()
{
    sqlite::Database database;
    QVERIFY_NOTHROW(database.open("testDatabase.sqlite"));
    QVERIFY_THROW(database.executeSimpleQuery("BAD SQL STATEMENT"), sqlite::Exception);
    QVERIFY_NOTHROW(database.close());
}

void Test::executeProperQuery()
{
    sqlite::Database database;
    QVERIFY_NOTHROW(database.open("testDatabase.sqlite"));

    sqlite::Query query(&database);
    QCOMPARE(query.isActive(), false);
    QVERIFY_NOTHROW(query.prepare("SELECT * FROM sqlite_master"));
    QCOMPARE(query.isActive(), true);
    while(query.step())
    {
        QCOMPARE(query.getColumnsCount(), 5);
    }
    QCOMPARE(query.isDone(), true);
    QVERIFY_NOTHROW(query.finalize());
    QCOMPARE(query.isActive(), false);
}

void Test::executeBadQuery()
{
    sqlite::Database database;
    QVERIFY_NOTHROW(database.open("testDatabase.sqlite"));

    sqlite::Query query(&database);
    QVERIFY_THROW(query.prepare("BAD SQL STATEMENT"), sqlite::Exception);
    QCOMPARE(query.isActive(), false);
    QVERIFY_NOTHROW(query.finalize());
}

void Test::createDeleteSimpleTree()
{
    tree::Node *root = new tree::Node;
    QCOMPARE(root->getChildrenCount(), 0);
    QVERIFY(root->getParent() == nullptr);
    QCOMPARE(root->getIndex(), 0);
    tree::Node *firstChild = new tree::Node;
    root->addChild(firstChild);
    QCOMPARE(root->getChildrenCount(), 1);
    QVERIFY(firstChild->getParent() == root);
    QCOMPARE(firstChild->getChildrenCount(), 0);
    QCOMPARE(firstChild->getIndex(), 0);
    tree::Node *secondChild = new tree::Node;
    root->addChild(secondChild);
    QCOMPARE(root->getChildrenCount(), 2);
    QVERIFY(secondChild->getParent() == root);
    QCOMPARE(secondChild->getChildrenCount(), 0);
    QCOMPARE(secondChild->getIndex(), 1);
    tree::Node *thirdChild = new tree::Node;
    root->addChild(thirdChild);
    QCOMPARE(root->getChildrenCount(), 3);
    QVERIFY(thirdChild->getParent() == root);
    QCOMPARE(thirdChild->getChildrenCount(), 0);
    QCOMPARE(thirdChild->getIndex(), 2);
    delete root;
}


QTEST_MAIN(Test)

#endif // TESTCASE
