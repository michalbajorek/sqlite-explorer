﻿#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

#include <QMainWindow>
#include "recordsetmodel.h"
#include "ribbon/ribbon.h"
#include "sqlite/database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadTable(QString tableName);
    void buttonOpenClicked();
    void buttonCloseClicked();

private:
    void trySetTableToModel(const QString &tableName);
    void loadTablesToCombo();
    void tryOpenDatabase(QString fileName);
    void showExceptionMessage(sqlite::Exception &exception);

    Ui::MainWindow *ui;
    RecordSetModel model;
    sqlite::Database database;
    ribbon::Ribbon *ribbonControl;
};

#endif // GUI_MAIN_WINDOW_H
