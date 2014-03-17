#include <QDebug>
#include <string>

#include "SqlSyntaxHighlighter.h"

using namespace parsing;

SqlSyntaxHighlighter::SqlSyntaxHighlighter(QObject *parent) :
    QSyntaxHighlighter(parent)
{
    mainKeywordFormat.setFontWeight(QFont::Bold);
    mainKeywordFormat.setForeground(QColor(Qt::red));

    keywordFormat.setFontWeight(QFont::Bold);
    keywordFormat.setForeground(QColor(Qt::darkGreen));

    stringFormat.setForeground(QColor(Qt::darkYellow));

    operatorFormat.setFontWeight(QFont::Bold);
    operatorFormat.setForeground(QColor(Qt::darkCyan));

    numberFormat.setForeground(QColor(Qt::magenta));

    commentFormat.setFontItalic(true);
    commentFormat.setForeground(QColor(Qt::lightGray));

    illegalFormat.setBackground(QColor(Qt::red));
    illegalFormat.setForeground(QColor(Qt::white));
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
            case Token::EndOfLine:
            case Token::Identifier:
                break;
            case Token::MultiLineCommentStart:
            case Token::MultiLineCommentEnd:
            case Token::MultiLineComment:
            case Token::SingleLineComment:
                setFormat(token->position, token->length, commentFormat);
                break;
            case Token::Minus:
            case Token::Plus:
            case Token::Star:
            case Token::Slash:
            case Token::LeftParen:
            case Token::RightParen:
            case Token::Semicolon:
            case Token::Reminder:
            case Token::Equals:
            case Token::NotEqual:
            case Token::Comma:
            case Token::BitAnd:
            case Token::BitNot:
            case Token::BitOr:
            case Token::Concat:
                setFormat(token->position, token->length, operatorFormat);
                break;
            case Token::Illegal:
                //setFormat(token->position, token->length, illegalFormat);
                break;
            case Token::MainKeyword:
                setFormat(token->position, token->length, mainKeywordFormat);
                break;
            case Token::Keyword:
                setFormat(token->position, token->length, keywordFormat);
                break;
            case Token::String:
                setFormat(token->position, token->length, stringFormat);
                break;
        }
    }
}
