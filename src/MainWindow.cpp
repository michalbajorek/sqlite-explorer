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
    ui->treeView->setModel(databaseList.getModel());
    ui->treeView->setSelectionMode(QTreeView::ExtendedSelection);
    ui->treeView->setSelectionBehavior(QTreeView::SelectRows);

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
catch(sqlite::Exception &exception)
{
    showExceptionMessage(exception);
}

void MainWindow::closeSelectedDatabase()
try
{
    recordSetModel.setRecordSet(NULL);
    QSet<QString> fileNames;
    getSelectedDatabaseFileNames(fileNames);
    closeDatabasesFromFileNameList(fileNames);
}
catch(sqlite::Exception &exception)
{
    showExceptionMessage(exception);
}

void MainWindow::getSelectedDatabaseFileNames(QSet<QString> &fileNames)
{
    QModelIndexList selectedIndexes = ui->treeView->selectionModel()->selectedIndexes();
    foreach(QModelIndex index, selectedIndexes)
    {
        TreeNode *node = (TreeNode*)index.internalPointer();
        if(TableNode *tableNode = dynamic_cast<TableNode*>(node))
            node = tableNode->getParent();
        if(DatabaseNode *databaseNode = dynamic_cast<DatabaseNode*>(node))
            fileNames.insert(databaseNode->getDatabase()->getFileName());
    }
}

void MainWindow::closeDatabasesFromFileNameList(QSet<QString> &fileNames)
{
    databaseList.beginUpdate();
    foreach(QString fileName, fileNames)
        databaseList.removeDatabase(fileName);
    databaseList.endUpdate();
    ui->treeView->expandAll();
}

void MainWindow::treeModelCurrentChanged(const QModelIndex &current, const QModelIndex &)
{
    TreeNode *node = (TreeNode*)current.internalPointer();
    if(TableNode *tableNode = dynamic_cast<TableNode*>(node))
        trySetTableToModel(tableNode->getTable());
    if(DatabaseNode *databaseNode = dynamic_cast<DatabaseNode*>(node))
        trySetTableToModel(databaseNode->getMasterTable());
}
