#include <QRegExp>

#include <algorithm>

#include "Database.h"
#include "Exception.h"
#include "RecordSet.h"

using namespace sqlite;

RecordSet::RecordSet(Database *database) : Object(database), mainQuery(database)
{
    init();
}

void RecordSet::init()
{
    recordsCount = 0;
    firstBuffer = NULL;
    secondBuffer = NULL;
    busy = false;
}

RecordSet::~RecordSet()
{
    delete firstBuffer;
    delete secondBuffer;
}

void RecordSet::setQueryText(const QString &queryText)
{
    this->queryText = queryText;
    prepareRecords();
}

int RecordSet::getRecordsCount() const
{
    checkIsLoaded();
    return recordsCount;
}

void RecordSet::prepareRecords()
{
    if(firstBuffer == NULL || secondBuffer == NULL)
        allocateRecordBuffers();
    resetRecordBuffers();
    if(executeCountQuery())
        prepareMainQuery();
}

void RecordSet::allocateRecordBuffers()
{
    firstBuffer = new RecordBuffer;
    secondBuffer = new RecordBuffer;
}

void RecordSet::resetRecordBuffers()
{
    firstBuffer->reset();
    secondBuffer->reset();
}

bool RecordSet::executeCountQuery()
{
    Query countQuery(database);
    countQuery.prepare(getCountQueryText());
    if(countQuery.step())
    {
        recordsCount = countQuery.getIntegerValue(0);
        return true;
    }
    return false;
}

QString RecordSet::getCountQueryText()
{
    checkQueryIsSelect();
    int fromClauseIndex = queryText.indexOf("FROM", Qt::CaseInsensitive);
    if(fromClauseIndex == -1)
        throw Exception("Missing FROM clause");
    return "SELECT COUNT(*) " + queryText.right(queryText.length() - fromClauseIndex);
}

void RecordSet::checkQueryIsSelect() const
{
    if(queryText.startsWith("SELECT", Qt::CaseInsensitive) == false)
        throw Exception("Not a SELECT statement");
}

QString RecordSet::getMainQueryText()
{
    QString mainQueryText = queryText;
    mainQueryText.append(" LIMIT ");
    mainQueryText.append(QString::number(RecordBuffer::getMaxCount() * bufferCount));
    mainQueryText.append(" OFFSET ?");
    return mainQueryText;
}

const Record &RecordSet::getRecord(int recordIndex)
{
    checkIsLoaded();
    checkIndexOutOfRange(recordIndex);
    if(firstBuffer->containsRecord(recordIndex))
        return firstBuffer->getRecord(recordIndex);
    else if(secondBuffer->containsRecord(recordIndex))
        return secondBuffer->getRecord(recordIndex);
    else
        return getRecordAndLoadBuffers(recordIndex);
}

 const Record &RecordSet::getRecordAndLoadBuffers(int recordIndex)
{
    int startIndex = RecordBuffer::getStartIndexFromRecordIndex(recordIndex);
    if(firstBuffer->isRecordJustBeforeBuffer(recordIndex))
    {
        swapBuffers();
        loadFirstBuffer(startIndex);
        return firstBuffer->getRecord(recordIndex);

    }
    else if(secondBuffer->isRecordJustAfterBuffer(recordIndex))
    {
        swapBuffers();
        loadSecondBuffer(startIndex);
        return secondBuffer->getRecord(recordIndex);
    }
    else
    {
        loadAllBuffers(startIndex);
        return firstBuffer->getRecord(recordIndex);
    }
}

void RecordSet::loadRecordBuffers(int startIndex, bool loadFirstBuffer, bool loadSecondBuffer)
{
    if(isBusy())
        throw Exception("Can not load records. Database is busy");
    busy = true;
    mainQuery.bindInteger(1, startIndex);
    if(loadFirstBuffer)
    {
        firstBuffer->loadRecordsFromQuery(startIndex, mainQuery);
        startIndex += RecordBuffer::getMaxCount();
    }
    if(loadSecondBuffer && mainQuery.isDone() == false)
        secondBuffer->loadRecordsFromQuery(startIndex, mainQuery);
    mainQuery.reset();
    busy = false;
}

void RecordSet::checkIndexOutOfRange(int recordIndex) const
{
    checkIsLoaded();
    if(recordIndex < 0 || recordIndex >= recordsCount)
        throw Exception("Index out of range");

}

void RecordSet::checkIsLoaded() const
{
    if(mainQuery.isActive() == false)
        throw Exception("Recordset is not loaded");
}
