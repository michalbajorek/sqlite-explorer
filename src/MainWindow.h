#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

#include <QMainWindow>
#include <QList>
#include <QSet>

#include "DatabaseList.h"
#include "DatabaseTreeModel.h"
#include "RecordSetModel.h"
#include "parsing/SqlSyntaxHighlighter.h"
#include "sqlite/Database.h"
#include "sqlite/ProgressHandler.h"

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
    void openDatabaseWithDialog();
    void closeSelectedDatabase();
    void treeModelCurrentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    void setupUi();
    void setupModels();
    void setSplitterInitialSizes();
    void connectSignals();
    void loadSettings();
    void saveSettings();
    void trySetTableToModel(sqlite::Table *table);
    void loadTablesToCombo();
    void tryOpenDatabases(const QStringList &fileNames);
    void showExceptionMessage(common::Exception &exception);
    void getSelectedDatabases(QSet<sqlite::Database*> &databaseSet);
    void closeDatabases(QSet<sqlite::Database*> &fileNames);

    Ui::MainWindow *ui;
    RecordSetModel recordSetModel;
    DatabaseTreeModel databaseModel;
    DatabaseList databaseList;
    parsing::SqlSyntaxHighlighter sqlHighlighter;
};

#endif // GUI_MAIN_WINDOW_H
