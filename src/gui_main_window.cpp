#include <QMessageBox>
#include <QTextCodec>
#include <QAbstractTableModel>

#include "gui_main_window.h"
#include "ui_gui_main_window.h"
#include "sqlite/database.h"
#include "sqlite/exception.h"
#include "sqlite/object.h"
#include "sqlite/recordset.h"
#include "sqlite/query.h"
#include "sqlite/table.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onProgressHandler(sqlite::Database *, bool &)
{
    QMessageBox::information(NULL, "ÓłóŁ", "Kupa");
}

void MainWindow::on_pushButton_clicked()
{
    try
    {
        sqlite::Database *database = new sqlite::Database;

        database->open("places.sqlite");

        sqlite::Table *table = database->getTable("moz_places");

        model = new RecordSetModel(table);
        ui->tableView->setModel(model);
    }
    catch(sqlite::Exception &e)
    {
        QMessageBox::warning(this, "Title", e.getErrorMessage());
    }
}
