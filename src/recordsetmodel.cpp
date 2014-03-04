#include "recordsetmodel.h"

RecordSetModel::RecordSetModel(sqlite::RecordSet *recordSet)
{
    this->recordSet = recordSet;
}

void RecordSetModel::setRecordSet(sqlite::RecordSet *recordSet)
{
    beginResetModel();
    this->recordSet = recordSet;
    endResetModel();
}


int RecordSetModel::rowCount(const QModelIndex &) const
{
    if(recordSet)
        return recordSet->getRecordsCount();
    return 0;
}

int RecordSetModel::columnCount(const QModelIndex &parent) const
{
    if(recordSet)
        return recordSet->getColumnsCount();
    return 0;
}

QVariant RecordSetModel::data(const QModelIndex &index, int role) const
{
    int rowIndex = index.row();
    int columnIndex = index.column();

    switch(role)
    {
        case Qt::DisplayRole:
            return getDataDisplayRole(rowIndex, columnIndex);
        default:
            return QVariant();
    }
}

QVariant RecordSetModel::headerData(int index, Qt::Orientation orientation, int role) const
{
    switch(role)
    {
        case Qt::DisplayRole:
            if(orientation == Qt::Horizontal)
                return getHeaderDisplayRole(index);
            else
                return QVariant(index + 1); // rowid
    }

    return QVariant();
}

QVariant RecordSetModel::getDataDisplayRole(int rowIndex, int columnIndex) const
{
    sqlite::Record &record = recordSet->getRecord(rowIndex);
    return QVariant(record[columnIndex]);
}

QVariant RecordSetModel::getHeaderDisplayRole(int columnIndex) const
{
    return recordSet->getColumnName(columnIndex);
}
