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

    void setTokenFormats();
private:
    void setupFormats();
    virtual void highlightBlock(const QString &text);
    void createParserTokenList(const QString &text);

    SqlParser parser;

    QTextCharFormat primaryKeywordFormat;
    QTextCharFormat secondaryKeywordFormat;
    QTextCharFormat stringFormat;
    QTextCharFormat operatorFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat illegalFormat;
};

} // namespace parsing

#endif // SQLSYNTAXHIGHLIGHTER_H
