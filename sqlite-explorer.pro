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

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
}

SOURCES +=\
    src/sqlite/sqlite3.c \
    src/sqlite/Database.cpp \
    src/sqlite/Object.cpp \
    src/sqlite/Exception.cpp \
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
    src/sqlite/Api.cpp


HEADERS  += \
    src/sqlite/sqlite3.h \
    src/sqlite/Database.h \
    src/sqlite/Exception.h \
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
    src/sqlite/Api.h


FORMS    += \
    forms/MainWindow.ui


testcase {
    QT += testlib
    SOURCES += src/tests/TestQuery.cpp
    HEADERS += src/tests/TestQuery.h
    DEFINES += TESTCASE
}
