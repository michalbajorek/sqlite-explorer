#include <QMessageBox>
#include <QTextCodec>
#include <QAbstractTableModel>

#include "gui_main_window.h"
#include "ui_gui_main_window.h"
#include "sqlite/database.h"
#include "sqlite/exception.h"
#include "sqlite/object.h"
#include "sqlite/recordset.h"
#include "sqlite/query.h"
#include "sqlite/table.h"

class Model : public QAbstractTableModel
{
public:
    Model(sqlite::RecordSet *recordSet)
    {
        this->recordSet = recordSet;
    }

    int rowCount(const QModelIndex &parent) const
    {
        return recordSet->getRecordsCount();
    }
    int columnCount(const QModelIndex &parent) const
    {
        return recordSet->getColumnsCount();
    }
    QVariant data(const QModelIndex &index, int role) const
    {
        if(role == Qt::DisplayRole)
        {
            sqlite::Record &record = recordSet->getRecord(index.row());
            return record[index.column()];
        }
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if(role == Qt::DisplayRole)
        {
            if(orientation == Qt::Horizontal)
                return recordSet->getColumnName(section);
            return QVariant(section + 1);
        }
        return QVariant();
    }
private:
    sqlite::RecordSet *recordSet;

};

Model *model;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onProgressHandler(sqlite::Database *, bool &)
{
    QMessageBox::information(NULL, "ÓłóŁ", "Kupa");
}

void MainWindow::on_pushButton_clicked()
{
    try
    {
        sqlite::Database *database = new sqlite::Database;

        database->open("places.sqlite");

        sqlite::Table *table = database->getTable("moz_places");

        model = new Model(table);
        ui->tableView->setModel(model);
    }
    catch(sqlite::Exception &e)
    {
        QMessageBox::warning(this, "Title", e.getErrorMessage());
    }
}
