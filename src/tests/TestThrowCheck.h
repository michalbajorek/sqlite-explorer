#ifndef TESTTHROWCHECK_H
#define TESTTHROWCHECK_H

#include <QtTest>

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

#endif // TESTTHROWCHECK_H
