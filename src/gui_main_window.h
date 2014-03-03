#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

#include <QMainWindow>
#include "sqlite/database.h"
#include "sqlite/progress_handler.h"
#include "recordsetmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public sqlite::ProgressObserver
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void onProgressHandler(sqlite::Database *database, bool &cancelOperation);
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    RecordSetModel *model;
};

#endif // GUI_MAIN_WINDOW_H
