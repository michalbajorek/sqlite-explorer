﻿#ifndef RECORDSET_H
#define RECORDSET_H

#include <QStringList>

#include "Query.h"
#include "Object.h"
#include "Record.h"

namespace sqlite
{

class Database;

class RecordSet : public Object
{
public:
    RecordSet(Database *database);
    RecordSet(Database *database, const QString &queryText);
    virtual ~RecordSet();

    void setQueryText(const QString &queryText);

    int getRecordsCount() const
        { return recordsCount; }

    const Record& getRecord(int recordIndex);

    int getColumnsCount() const
        { return mainQuery.getColumnsCount(); }

    QString getColumnName(int columnIndex) const
        { return mainQuery.getColumnName(columnIndex); }

private:
    void init();
    void prepareRecords();
    void resetRecordBuffers();
    void allocateRecordBuffers();
    bool executeCountQuery();
    const Record &getRecordAndLoadBuffers(int recordIndex);
    QString getCountQueryText();
    QString getMainQueryText();

    void prepareMainQuery()
        { mainQuery.prepare(getMainQueryText()); }

    void checkQueryIsSelect() const;

    bool checkIndexOutOfRange(int recordIndex) const
        { return recordIndex < 0 || recordIndex >= recordsCount; }

    void swapBuffers()
        { std::swap(firstBuffer, secondBuffer); }

    void loadRecordBuffers(int startIndex, bool loadFirstBuffer, bool loadSecondBuffer);

    void loadFirstBuffer(int startIndex)
        { loadRecordBuffers(startIndex, true, false); }

    void loadSecondBuffer(int startIndex)
        { loadRecordBuffers(startIndex, false, true); }

    void loadAllBuffers(int startIndex)
        { loadRecordBuffers(startIndex, true, true); }

    int recordsCount;
    QString queryText;
    Query mainQuery;
    RecordBuffer *firstBuffer;
    RecordBuffer *secondBuffer;

    static const int bufferCount = 2;
};

} // namespace sqlite

#endif // RECORDSET_H