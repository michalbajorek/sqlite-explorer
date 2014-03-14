#ifndef RECORDSETMODEL_H
#define RECORDSETMODEL_H

#include <QAbstractTableModel>

#include "sqlite/RecordSet.h"

class RecordSetModel : public QAbstractTableModel
{
public:
    RecordSetModel();

    void setRecordSet(sqlite::RecordSet *newRecordSet);
    void clearRecordSet()
        { setRecordSet(nullptr); }

private:
    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int index, Qt::Orientation orientation, int role) const;

    QVariant getDataDisplayRole(int rowIndex, int columnIndex) const;
    QVariant getHeaderDisplayRole(int columnIndex) const;

    sqlite::RecordSet *recordSet;

    static const int MaxFieldLength = 1000;
};

#endif // RECORDSETMODEL_H
