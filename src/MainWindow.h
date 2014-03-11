#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

#include <QMainWindow>

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

private slots:
    void buttonOpenClicked();
    void closeDatabase();
    void treeModelCurrentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    void connectSignals();
    void setSplitterInitialSizes();
    void loadSettings();
    void saveSettings();
    void trySetTableToModel(sqlite::Table *table);
    void loadTablesToCombo();
    void tryOpenDatabase(QString fileName);
    void showExceptionMessage(sqlite::Exception &exception);

    Ui::MainWindow *ui;
    RecordSetModel recordSetModel;
    DatabaseTreeModel databaseModel;
    sqlite::Database database;
};

#endif // GUI_MAIN_WINDOW_H
