#include <QRegExp>

#include <algorithm>

#include "database.h"
#include "exception.h"
#include "recordset.h"

using namespace sqlite;

RecordSet::RecordSet(Database *database) : Object(database), mainQuery(database)
{
    init();
}

RecordSet::RecordSet(Database *database, const QString &queryText) : Object(database), mainQuery(database)
{
    init();
    setQueryText(queryText);
}

void RecordSet::init()
{
    recordsCount = 0;
    firstBuffer = NULL;
    secondBuffer = NULL;
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
    Query countQuery(database, getCountQueryText());
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
    QString mainQuery = queryText;
    mainQuery.append(" LIMIT ");
    mainQuery.append(QString::number(RecordBuffer::getMaxCount() * bufferCount));
    mainQuery.append(" OFFSET ?");
    return mainQuery;
}

const Record &RecordSet::getRecord(int recordIndex)
{
    if(checkIndexOutOfRange(recordIndex))
        throw Exception("Index out of range");
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
    mainQuery.bindInteger(1, startIndex);
    if(loadFirstBuffer)
    {
        firstBuffer->loadRecordsFromQuery(startIndex, mainQuery);
        startIndex += RecordBuffer::getMaxCount();
    }
    if(loadSecondBuffer && mainQuery.isDone() == false)
        secondBuffer->loadRecordsFromQuery(startIndex, mainQuery);
    mainQuery.reset();
}
