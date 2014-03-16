#ifndef SQLSYNTAXHIGHLIGHTER_H
#define SQLSYNTAXHIGHLIGHTER_H

#include <QString>
#include <QSyntaxHighlighter>

#include "SqlParser.h"

namespace parsing
{

class SqlSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit SqlSyntaxHighlighter(QObject *parent = 0);

private:
    virtual void highlightBlock(const QString &text);

    SqlParser parser;

    QTextCharFormat keywordFormat;
    QTextCharFormat stringFormat;
    QTextCharFormat operatorFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat commentFormat;
};

} // namespace parsing

#endif // SQLSYNTAXHIGHLIGHTER_H
