#-------------------------------------------------
#
# Project created by QtCreator 2014-02-21T22:53:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

TARGET = SqliteExplorer
TEMPLATE = app
LIBS += -ldl
CONFIG += c++11

#CONFIG(release) {
#    QMAKE_CXXFLAGS += -O3
#}

SOURCES += \
    src/sqlite/sqlite3.c \
    src/sqlite/Api.cpp \
    src/sqlite/Database.cpp \
    src/sqlite/Object.cpp \
    src/sqlite/ProgressHandler.cpp \
    src/sqlite/Query.cpp \
    src/sqlite/Record.cpp \
    src/sqlite/RecordSet.cpp \
    src/sqlite/Table.cpp \
    src/sqlite/Tables.cpp \
    src/Application.cpp \
    src/MainWindow.cpp \
    src/Initialization.cpp \
    src/Main.cpp \
    src/RecordSetModel.cpp \
    src/Settings.cpp \
    src/sqlite/QueryLogger.cpp \
    src/DatabaseTreeModel.cpp \
    src/DatabaseList.cpp \
    src/sqlite/DatabaseRepository.cpp \
    src/tree/Node.cpp \
    src/tree/DatabaseNode.cpp \
    src/tree/TableNode.cpp \
    src/parsing/SqlParser.cpp \
    src/parsing/SqlSyntaxHighlighter.cpp


HEADERS  += \
    src/sqlite/sqlite3.h \
    src/sqlite/Api.h \
    src/sqlite/Database.h \
    src/sqlite/Object.h \
    src/sqlite/ProgressHandler.h \
    src/sqlite/Query.h \
    src/sqlite/Record.h \
    src/sqlite/RecordSet.h \
    src/sqlite/Table.h \
    src/sqlite/Tables.h \
    src/Application.h \
    src/MainWindow.h \
    src/Initialization.h \
    src/RecordSetModel.h \
    src/Settings.h \
    src/sqlite/QueryLogger.h \
    src/DatabaseTreeModel.h \
    src/DatabaseList.h \
    src/sqlite/DatabaseRepository.h \
    src/tree/Node.h \
    src/tree/DatabaseNode.h \
    src/tree/TableNode.h \
    src/common/Exception.h \
    src/parsing/SqlParser.h \
    src/parsing/SqlSyntaxHighlighter.h

FORMS    += \
    forms/MainWindow.ui


testcase {
    QT += testlib
    SOURCES += src/tests/Test.cpp
    HEADERS += src/tests/TestThrowCheck.h \
               src/tests/Test.h
    DEFINES += TESTCASE
}

RESOURCES += \
    res/Resources.qrc
