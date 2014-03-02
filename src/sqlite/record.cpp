#include "record.h"

using namespace sqlite;

RecordBuffer::RecordBuffer()
{
    buffer = new Record[maxCount];
    reset();
}

RecordBuffer::~RecordBuffer()
{
    delete [] buffer;
}

void RecordBuffer::reset()
{
    startIndex = 0;
    loadedCount = 0;
}

void RecordBuffer::loadRecordsFromQuery(int startIndex, Query &query)
{
    this->startIndex = startIndex;
    this->loadedCount = 0;
    while(hasEmptySpace())
    {
        if(query.step())
            loadNextRecordFromQuery(query);
        else
            break;
    }
}

void RecordBuffer::loadNextRecordFromQuery(Query &query)
{
    Record &record = buffer[loadedCount++];
    record.initRecordFromQuery(query);
}

void Record::initRecordFromQuery(Query &query)
{
    clear();
    rowId = query.getInt64Value(0);
    int columnsCount = query.getColumnsCount();
    for(int columnIndex = 1; columnIndex < columnsCount; columnIndex++)
        append(query.getTextValue(columnIndex));
}
