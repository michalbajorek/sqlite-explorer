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
    ui->tableView->setModel(&recordSetModel);
    ui->treeView->setModel(&databaseModel);

    setSplitterInitialSizes();
    connectSignals();

    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::setSplitterInitialSizes()
{
    QList<int> sizes;
    sizes << 100 << 300;
    ui->splitter->setSizes(sizes);
}

void MainWindow::connectSignals()
{
    connect(ui->actionOpenDatabase, &QAction::triggered, this, &MainWindow::openDatabaseWithDialog);
    connect(ui->actionCloseDatabase, &QAction::triggered, this, &MainWindow::closeDatabase);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::treeModelCurrentChanged);
}

void MainWindow::loadSettings()
{
    globalSettings.loadWindowGeometry(this);
    globalSettings.loadSplitterState(ui->splitter);
}

void MainWindow::saveSettings()
{
    globalSettings.saveWindowGeometry(this);
    globalSettings.saveSplitterState(ui->splitter);
}

void MainWindow::trySetTableToModel(sqlite::Table *table)
try
{
    if(table->isLoaded() == false)
        table->loadContent();
    recordSetModel.setRecordSet(table);
}
catch(sqlite::Exception &exception)
{
    showExceptionMessage(exception);
}

void MainWindow::showExceptionMessage(sqlite::Exception &exception)
{
    QMessageBox::warning(this, "Title", exception.getErrorMessage());
}

void MainWindow::openDatabaseWithDialog()
{
    QFileDialog fileDialog(this);
    fileDialog.setNameFilter("Sqlite database (*.sqlite)");
    if(fileDialog.exec())
    {
        closeDatabase();
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
    databaseModel.setDatabase(&database);
}
catch(sqlite::Exception &exception)
{
    showExceptionMessage(exception);
}

void MainWindow::closeDatabase()
{
    if(database.isOpened())
    {
        recordSetModel.clearRecordSet();
        databaseModel.clearDatabase();
        database.close();
    }
}

void MainWindow::treeModelCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    TreeNode* node = (TreeNode*)current.internalPointer();
    if(TableNode *tableNode = dynamic_cast<TableNode*>(node))
        trySetTableToModel(tableNode->getTable());
    if(DatabaseNode *databaseNode = dynamic_cast<DatabaseNode*>(node))
        trySetTableToModel(databaseNode->getMasterTable());
}
