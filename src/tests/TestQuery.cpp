#include <QString>

#ifdef TESTCASE

#include "TestQuery.h"
#include "../sqlite/Query.h"

#define QVERIFY_THROW(expression, expectedExceptionType) \
   do { \
       bool caught = false; \
       try { expression; } catch (expectedExceptionType const&) { caught = true; } catch (...) {} \
       if (!QTest::qVerify(caught, "", "expecting " #expression " to throw <" #expectedExceptionType ">", __FILE__, __LINE__))\
            return; \
   } while(0)

#define QVERIFY_NOTHROW(expression) \
   do { \
       bool thrown = false; \
       try { expression; } catch (...) { thrown = true; } \
       if (!QTest::qVerify(!thrown, "", #expression " throws exception", __FILE__, __LINE__))\
            return; \
   } while(0)

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


QTEST_MAIN(Test)

#endif // TESTCASE
