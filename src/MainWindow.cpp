#include <QMessageBox>
#include <QAbstractTableModel>
#include <QFileDialog>

#include "MainWindow.h"
#include "Settings.h"
#include "common/Exception.h"
#include "sqlite/Database.h"
#include "sqlite/Object.h"
#include "sqlite/RecordSet.h"
#include "sqlite/Query.h"
#include "sqlite/Table.h"
#include "tree/DatabaseNode.h"
#include "tree/TableNode.h"

#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setupUi();
    connectSignals();
    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::setupUi()
{
    ui->setupUi(this);
    setupModels();
    setSplitterInitialSizes();
}

void MainWindow::setupModels()
{
    ui->tableView->setModel(&recordSetModel);
    ui->treeView->setModel(databaseList.getModel());
    ui->treeView->setSelectionMode(QTreeView::ExtendedSelection);
    ui->treeView->setSelectionBehavior(QTreeView::SelectRows);
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
    connect(ui->actionCloseDatabase, &QAction::triggered, this, &MainWindow::closeSelectedDatabase);
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
catch(common::Exception &exception)
{
    showExceptionMessage(exception);
}

void MainWindow::showExceptionMessage(common::Exception &exception)
{
    QMessageBox::warning(this, "Title", exception.getErrorMessage());
}

void MainWindow::openDatabaseWithDialog()
{
    QFileDialog fileDialog(this);
    fileDialog.setNameFilter("Sqlite database (*.sqlite)");
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    if(fileDialog.exec())
        tryOpenDatabases(fileDialog.selectedFiles());
}

void MainWindow::tryOpenDatabases(const QStringList &fileNames)
try
{
    databaseList.beginUpdate();
    foreach(QString fileName, fileNames)
        databaseList.addDatabase(fileName);
    databaseList.endUpdate();
    ui->treeView->expandAll();
}
catch(common::Exception &exception)
{
    showExceptionMessage(exception);
}

void MainWindow::closeSelectedDatabase()
try
{
    recordSetModel.clearRecordSet();
    QSet<sqlite::Database*> databaseSet;
    getSelectedDatabases(databaseSet);
    closeDatabases(databaseSet);
}
catch(common::Exception &exception)
{
    showExceptionMessage(exception);
}

void MainWindow::getSelectedDatabases(QSet<sqlite::Database*> &databaseSet)
{
    QModelIndexList selectedIndexes = ui->treeView->selectionModel()->selectedIndexes();
    foreach(QModelIndex index, selectedIndexes)
    {
        tree::DatabaseNode *node = nullptr;
        if(index.parent().isValid())
            node = static_cast<tree::DatabaseNode*>(index.parent().internalPointer());
        else
            node = static_cast<tree::DatabaseNode*>(index.internalPointer());
        databaseSet.insert(node->getDatabase());
    }
}

void MainWindow::closeDatabases(QSet<sqlite::Database*> &databaseSet)
{
    databaseList.beginUpdate();
    foreach(sqlite::Database *database, databaseSet)
        databaseList.removeDatabase(database);
    databaseList.endUpdate();
    ui->treeView->expandAll();
}

void MainWindow::treeModelCurrentChanged(const QModelIndex &current, const QModelIndex &)
{
    tree::TableNode *node = static_cast<tree::TableNode*>(current.internalPointer());
    trySetTableToModel(node->getTable());
}
