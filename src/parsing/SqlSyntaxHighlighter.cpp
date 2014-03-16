#include <QDebug>
#include <string>

#include "SqlSyntaxHighlighter.h"

using namespace parsing;

SqlSyntaxHighlighter::SqlSyntaxHighlighter(QObject *parent) :
    QSyntaxHighlighter(parent)
{
    keywordFormat.setFontWeight(QFont::Bold);
    keywordFormat.setForeground(QColor(0x52, 0xaa, 0x80));

    stringFormat.setForeground(QColor(Qt::darkYellow));

    operatorFormat.setFontWeight(QFont::Bold);
    operatorFormat.setForeground(QColor(Qt::darkCyan));

    numberFormat.setForeground(QColor(Qt::magenta));

    commentFormat.setFontItalic(true);
    commentFormat.setForeground(QColor(Qt::lightGray));
}


void SqlSyntaxHighlighter::highlightBlock(const QString &text)
{
    parser.setLastTokenType(previousBlockState());
    parser.setQueryText(text);
    setCurrentBlockState(parser.getLastTokenType());

    foreach(Token *token, parser.tokenList)
    {
        switch(token->type)
        {
            case Token::None:
            case Token::Space:
                break;
            case Token::MultiLineCommentStart:
            case Token::MultiLineCommentEnd:
            case Token::MultiLineComment:
            case Token::SingleLineComment:
                setFormat(token->position, token->length, commentFormat);
                break;
            case Token::Minus:
            case Token::LeftParen:
            case Token::RightParen:
            case Token::Semicolon:
            case Token::Plus:
            case Token::Star:
            case Token::Reminder:
            case Token::Equals:
            case Token::Comma:
            case Token::BitAnd:
            case Token::BitNot:
            case Token::Slash:
                setFormat(token->position, token->length, operatorFormat);
                break;
        }
    }
}
