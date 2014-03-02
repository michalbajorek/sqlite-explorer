#ifndef RECORD_H
#define RECORD_H

#include <QStringList>

#include "query.h"

namespace sqlite
{

class Record : public QStringList
{
public:
    Record()
        { rowId = 0; }

    int64_t getRowId()
        { return rowId; }

    void initRecordFromQuery(Query &query);

private:
    int64_t rowId;
};

class RecordBuffer
{
public:
    RecordBuffer();
    ~RecordBuffer();

    void reset();
    void loadRecordsFromQuery(int startIndex, Query &query);
    void loadNextRecordFromQuery(Query &query);

    bool hasEmptySpace()
        { return loadedCount < maxCount; }

    int getLoadedCount()
        { return loadedCount; }

    bool containsRecord(int recordIndex)
        { return recordIndex >= startIndex && recordIndex < loadedCount; }

    bool isRecordJustAfterBuffer(int recordIndex)
        { return recordIndex >= startIndex + maxCount && recordIndex < startIndex + 2 * maxCount; }

    bool isRecordJustBeforeBuffer(int recordIndex)
        { return recordIndex >= startIndex - maxCount && recordIndex < startIndex; }

    Record& getRecord(int recordIndex)
        { return buffer[recordIndex]; }

    static int getMaxCount()
        { return maxCount; }

    static int getStartIndexFromRecordIndex(int recordIndex)
        { return (recordIndex / maxCount) * maxCount; }

private:
    Record *buffer;
    int startIndex;
    int loadedCount;

    static const int maxCount = 100;
};

} // namespace sqlite

#endif // RECORD_H
