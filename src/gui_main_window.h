#ifndef GUI_MAIN_WINDOW_H
#define GUI_MAIN_WINDOW_H

#include <QMainWindow>
#include "sqlite/database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //void progressHandler(sqlite::Database *database, bool &cancelOperation);
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // GUI_MAIN_WINDOW_H
