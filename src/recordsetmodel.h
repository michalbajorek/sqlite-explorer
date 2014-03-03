#ifndef RECORDSETMODEL_H
#define RECORDSETMODEL_H

#include <QAbstractTableModel>

#include "sqlite/recordset.h"


class RecordSetModel : public QAbstractTableModel
{
public:
    RecordSetModel(sqlite::RecordSet *recordSet);

private:
    sqlite::RecordSet *recordSet;

private:
    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int index, Qt::Orientation orientation, int role) const;

    QVariant getDataDisplayRole(int rowIndex, int columnIndex) const;
    QVariant getHeaderDisplayRole(int columnIndex) const;
};

#endif // RECORDSETMODEL_H
