#include <QMessageBox>
#include <QTextCodec>

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
        sqlite::Database database;

        database.open("places.sqlite");
        database.progressHandler.addObserver(this);
        database.progressHandler.setOperationInterval(100);

        sqlite::Table *table = database.getTable("sqlite_master");

        QMessageBox::information(this, "Info", QString::number(table->getRecordsCount()));
        QMessageBox::information(this, "Info", table->getRecord(0).at(1));
    }
    catch(sqlite::Exception &e)
    {
        QMessageBox::warning(this, "Title", e.getErrorMessage());
    }
}
