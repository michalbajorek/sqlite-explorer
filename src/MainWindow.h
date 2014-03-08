#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

#include <QMainWindow>
#include "RecordSetModel.h"
#include "sqlite/Database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void connectSignals();
private slots:
    void loadTable(QString tableName);
    void buttonOpenClicked();
    void buttonCloseClicked();

private:
    void trySetTableToModel(const QString &tableName);
    void loadTablesToCombo();
    void tryOpenDatabase(QString fileName);
    void showExceptionMessage(sqlite::Exception &exception);

    Ui::MainWindow *ui;
    RecordSetModel model;
    sqlite::Database database;
};

#endif // GUI_MAIN_WINDOW_H
