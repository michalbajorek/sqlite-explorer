#ifndef RECORD_H
#define RECORD_H

#include <cassert>

#include <QStringList>

#include "query.h"

namespace sqlite
{

class Record
{
public:
    Record()
        { }

    int getCount() const
        { return fieldList.count(); }

    const QString &getField(int index) const
        { return fieldList.at(index); }

    void initRecordFromQuery(Query &query);
private:
    QStringList fieldList;
};

class RecordBuffer
{
public:
    RecordBuffer();
    ~RecordBuffer();

    void reset();
    void loadRecordsFromQuery(int startIndex, Query &query);
    void loadNextRecordFromQuery(Query &query);

    bool hasEmptySpace() const
        { return loadedCount < maxCount; }

    int getLoadedCount() const
        { return loadedCount; }

    bool containsRecord(int recordIndex) const
        { return recordIndex >= startIndex && recordIndex < startIndex + loadedCount; }

    bool isRecordJustAfterBuffer(int recordIndex) const
        { return recordIndex >= startIndex + maxCount && recordIndex < startIndex + 2 * maxCount; }

    bool isRecordJustBeforeBuffer(int recordIndex) const
        { return recordIndex >= startIndex - maxCount && recordIndex < startIndex; }

     const Record& getRecord(int recordIndex) const
    {
        assert(containsRecord(recordIndex));
        return buffer[recordIndex - startIndex];
    }

    static int getMaxCount()
        { return maxCount; }

    static int getStartIndexFromRecordIndex(int recordIndex)
        { return (recordIndex / maxCount) * maxCount; }

private:
    Record *buffer;
    int startIndex;
    int loadedCount;

    static const int maxCount = 500;
};

} // namespace sqlite

#endif // RECORD_H
