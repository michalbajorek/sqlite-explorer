#include "Record.h"

using namespace sqlite;

void Record::initRecordFromQuery(Query &query)
{
    fieldList.clear();
    int columnsCount = query.getColumnsCount();
    for(int columnIndex = 0; columnIndex < columnsCount; columnIndex++)
        fieldList.append(query.getTextValue(columnIndex));
}

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
