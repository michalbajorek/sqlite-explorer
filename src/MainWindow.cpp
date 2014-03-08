#include <QMessageBox>
#include <QTextCodec>
#include <QAbstractTableModel>
#include <QFileDialog>
#include <QSettings>

#include "gui_main_window.h"
#include "settings.h"
#include "sqlite/database.h"
#include "sqlite/exception.h"
#include "sqlite/object.h"
#include "sqlite/recordset.h"
#include "sqlite/query.h"
#include "sqlite/table.h"

#include "ui_gui_main_window.h"

QSettings settings;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(&model);

    connect(ui->comboTables, SIGNAL(currentIndexChanged(QString)), this, SLOT(loadTable(QString)));
    connect(ui->buttonOpen, SIGNAL(clicked()), this, SLOT(buttonOpenClicked()));
    connect(ui->buttonClose, SIGNAL(clicked()), this, SLOT(buttonCloseClicked()));

    globalSettings.loadWindowGeometry(this);
}

MainWindow::~MainWindow()
{
    globalSettings.saveWindowGeometry(this);
    delete ui;
}

void MainWindow::loadTable(QString tableName)
{
    if(tableName.isEmpty())
        return;
    trySetTableToModel(tableName);
}

void MainWindow::trySetTableToModel(const QString &tableName)
try
{
    sqlite::Table *table = database.tables.getTable(tableName);
    model.setRecordSet(table);
}
catch(sqlite::Exception &exception)
{
    showExceptionMessage(exception);
}

void MainWindow::showExceptionMessage(sqlite::Exception &exception)
{
    QMessageBox::warning(this, "Title", exception.getErrorMessage());
}

void MainWindow::buttonOpenClicked()
{
    QFileDialog fileDialog(this);

    fileDialog.setNameFilter("Sqlite database (*.sqlite)");
    if(fileDialog.exec())
    {
        QString fileName = fileDialog.selectedFiles().first();
        tryOpenDatabase(fileName);
    }
}

void MainWindow::tryOpenDatabase(QString fileName)
try
{
    if(database.isOpened())
        database.close();
    database.open(fileName);
    loadTablesToCombo();
}
catch(sqlite::Exception &exception)
{
    showExceptionMessage(exception);
}

void MainWindow::loadTablesToCombo()
{
    ui->comboTables->clear();
    for(int i = 0; i < database.tables.getCount(); i++)
    {
        sqlite::Table *table = database.tables.getTable(i);
        ui->comboTables->addItem(table->getName());
    }
}

void MainWindow::buttonCloseClicked()
{
    model.clearRecordSet();
    ui->comboTables->clear();
    database.close();
}
