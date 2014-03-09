#include <QMessageBox>
#include <QTextCodec>
#include <QAbstractTableModel>
#include <QFileDialog>
#include <QSettings>

#include "MainWindow.h"
#include "Settings.h"
#include "sqlite/Database.h"
#include "sqlite/Exception.h"
#include "sqlite/Object.h"
#include "sqlite/RecordSet.h"
#include "sqlite/Query.h"
#include "sqlite/Table.h"

#include "ui_MainWindow.h"

QSettings settings;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(&model);

    connectSignals();

    globalSettings.loadWindowGeometry(this);
}

MainWindow::~MainWindow()
{
    globalSettings.saveWindowGeometry(this);
    delete ui;
}

void MainWindow::connectSignals()
{
    connect(ui->comboTables, SIGNAL(currentIndexChanged(QString)), this, SLOT(loadTable(QString)));
    connect(ui->actionOpenDatabase, &QAction::triggered, this, &MainWindow::buttonOpenClicked);
    connect(ui->actionCloseDatabase, &QAction::triggered, this, &MainWindow::buttonCloseClicked);
    connect(&database.progressHandler, &sqlite::ProgressHandler::progress, this, &MainWindow::progressHandler);
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
    if(table->isLoaded() == false)
        table->loadContent();
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
    if(database.isOpened())
    {
        model.clearRecordSet();
        ui->comboTables->clear();
        database.close();
    }
}


void MainWindow::progressHandler(sqlite::Database *, bool &)
{
}
