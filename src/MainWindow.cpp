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
    ui(new Ui::MainWindow),
    sqlHighlighter(this)
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
    sqlHighlighter.setDocument(ui->textEdit->document());
}

void MainWindow::setupModels()
{
    ui->tableView->setModel(&recordSetModel);
    ui->tableView->setTextElideMode(Qt::ElideNone);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(25);

    ui->treeView->setModel(databaseList.getModel());
    ui->treeView->setSelectionMode(QTreeView::ExtendedSelection);
    ui->treeView->setSelectionBehavior(QTreeView::SelectRows);
}

void MainWindow::setSplitterInitialSizes()
{
    QList<int> sizes;
    sizes << 100 << 300;
    ui->splitterMain->setSizes(sizes);
    sizes.clear();
    sizes << 50 << 350;
    ui->splitterText->setSizes(sizes);
}

void MainWindow::connectSignals()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    connect(ui->actionOpenDatabase, SIGNAL(triggered()), this, SLOT(openDatabaseWithDialog()));
    connect(ui->actionCloseDatabase, SIGNAL(triggered()), this, SLOT(closeSelectedDatabase()));
    connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(treeModelCurrentChanged(QModelIndex,QModelIndex)));
#else
    connect(ui->actionOpenDatabase, &QAction::triggered, this, &MainWindow::openDatabaseWithDialog);
    connect(ui->actionCloseDatabase, &QAction::triggered, this, &MainWindow::closeSelectedDatabase);
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::treeModelCurrentChanged);
#endif
}

void MainWindow::loadSettings()
{
    globalSettings.loadWindowGeometry(this);
    globalSettings.loadSplitterState(ui->splitterMain);
    globalSettings.loadSplitterState(ui->splitterText);
}

void MainWindow::saveSettings()
{
    globalSettings.saveWindowGeometry(this);
    globalSettings.saveSplitterState(ui->splitterMain);
    globalSettings.saveSplitterState(ui->splitterText);
}

void MainWindow::trySetTableToModel(sqlite::Table *table)
try
{
    if(table->isLoaded() == false)
        table->loadContent();
    recordSetModel.setRecordSet(table);
    //ui->tableView->resizeRowsToContents();
//    ui->tableView->resizeColumnsToContents();
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
    tree::ViewNode *node = static_cast<tree::ViewNode*>(current.internalPointer());
    trySetTableToModel(node->getTable());
}
