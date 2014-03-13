#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

#include <QMainWindow>
#include <QList>
#include <QSet>

#include "DatabaseList.h"
#include "DatabaseTreeModel.h"
#include "RecordSetModel.h"
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

    void closeDatabasesFromFileNameList(QSet<QString> &fileNames);
private slots:
    void openDatabaseWithDialog();
    void closeSelectedDatabase();
    void treeModelCurrentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    void connectSignals();
    void setSplitterInitialSizes();
    void loadSettings();
    void saveSettings();
    void trySetTableToModel(sqlite::Table *table);
    void loadTablesToCombo();
    void tryOpenDatabases(const QStringList &fileNames);
    void showExceptionMessage(sqlite::Exception &exception);
    void getSelectedDatabaseFileNames(QSet<QString> &fileNames);

    Ui::MainWindow *ui;
    RecordSetModel recordSetModel;
    DatabaseTreeModel databaseModel;
    DatabaseList databaseList;
};

#endif // GUI_MAIN_WINDOW_H
