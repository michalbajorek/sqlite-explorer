#-------------------------------------------------
#
# Project created by QtCreator 2014-02-21T22:53:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SqliteExplorer
TEMPLATE = app
LIBS += -ldl
CONFIG(debug, release|debug):DEFINES += _DEBUG
CONFIG += c++11

SOURCES += src/main.cpp\
    src/gui_main_window.cpp \
    src/sqlite/table.cpp \
    src/sqlite/sqlite3.c \
    src/sqlite/database.cpp \
    src/sqlite/object.cpp \
    src/sqlite/exception.cpp \
    src/sqlite/query.cpp \
    src/sqlite/recordset.cpp \
    src/sqlite/record.cpp \
    src/sqlite/progress_handler.cpp \
    src/sqlite/tables.cpp \
    src/initialization.cpp \
    src/application.cpp \
    src/recordsetmodel.cpp \
    src/settings.cpp

HEADERS  += src/gui_main_window.h \
    src/sqlite/sqlite3.h \
    src/sqlite/database.h \
    src/sqlite/table.h \
    src/sqlite/object.h \
    src/sqlite/exception.h \
    src/sqlite/query.h \
    src/sqlite/recordset.h \
    src/sqlite/record.h \
    src/sqlite/progress_handler.h \
    src/sqlite/tables.h \
    src/initialization.h \
    src/application.h \
    src/recordsetmodel.h \
    src/settings.h \
    src/ribbon/ribbon.h \
    src/ribbon/tabs.h \
    src/ribbon/tab.h

FORMS    += forms/gui_main_window.ui
