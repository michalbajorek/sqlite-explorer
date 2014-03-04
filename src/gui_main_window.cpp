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
    ui->tableView->setModel(&model);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(loadTable(QString)));
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
        database.open("places.sqlite");
        loadTablesToCombo();
    }
    catch(sqlite::Exception &e)
    {
        QMessageBox::warning(this, "Title", e.getErrorMessage());
    }
}

void MainWindow::loadTablesToCombo()
{
    for(int i = 0; i < database.tables.getCount(); i++)
    {
        sqlite::Table *table = database.tables.getTable(i);
        ui->comboBox->addItem(table->getName());
    }
}

void MainWindow::loadTable(QString tableName)
{
    try
    {
        sqlite::Table *table = database.tables.getTable(tableName);
        model.setRecordSet(table);
    }
    catch(sqlite::Exception &e)
    {
        QMessageBox::warning(this, "Title", e.getErrorMessage());
    }
}
