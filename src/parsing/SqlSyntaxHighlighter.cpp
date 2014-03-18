#include <QDebug>
#include <string>

#include "SqlSyntaxHighlighter.h"

using namespace parsing;

SqlSyntaxHighlighter::SqlSyntaxHighlighter(QObject *parent) :
    QSyntaxHighlighter(parent)
{
    setupFormats();
}

void SqlSyntaxHighlighter::setupFormats()
{
    primaryKeywordFormat.setForeground(QColor(Qt::blue));
    secondaryKeywordFormat.setForeground(QColor(Qt::darkCyan));
    stringFormat.setForeground(QColor(Qt::darkRed));
    operatorFormat.setForeground(QColor(Qt::darkGray));
    numberFormat.setForeground(QColor(Qt::magenta));
    commentFormat.setForeground(QColor(Qt::lightGray));
    illegalFormat.setBackground(QColor(Qt::red));
    illegalFormat.setForeground(QColor(Qt::white));
    variableFormat.setForeground(QColor(Qt::darkMagenta));
}

void SqlSyntaxHighlighter::highlightBlock(const QString &text)
{
    createParserTokenList(text);
    setTokenFormats();
}

void SqlSyntaxHighlighter::createParserTokenList(const QString &text)
{
    TokenType lastTokenType = static_cast<TokenType>(previousBlockState());
    parser.setTextAndLastTokenType(text, lastTokenType);
    setCurrentBlockState(int(parser.getLastTokenType()));
}

void SqlSyntaxHighlighter::setTokenFormats()
{
    foreach(Token *token, parser.tokenList)
    {
        switch(token->type)
        {
            case TokenType::None:
            case TokenType::Space:
            case TokenType::EndOfLine:
            case TokenType::Identifier:
                break;
            case TokenType::MultiLineCommentStart:
            case TokenType::MultiLineCommentEnd:
            case TokenType::MultiLineComment:
            case TokenType::SingleLineComment:
                setFormat(token->position, token->length, commentFormat);
                break;
            case TokenType::Minus:
            case TokenType::Plus:
            case TokenType::Star:
            case TokenType::Slash:
            case TokenType::LeftParen:
            case TokenType::RightParen:
            case TokenType::Semicolon:
            case TokenType::Reminder:
            case TokenType::Equals:
            case TokenType::NotEqual:
            case TokenType::Comma:
            case TokenType::BitAnd:
            case TokenType::BitNot:
            case TokenType::BitOr:
            case TokenType::Concat:
            case TokenType::LeftShift:
            case TokenType::RightShift:
            case TokenType::LessThan:
            case TokenType::GreaterThan:
            case TokenType::LessOrEqual:
            case TokenType::GreaterEqual:
                setFormat(token->position, token->length, operatorFormat);
                break;
            case TokenType::Illegal:
                setFormat(token->position, token->length, illegalFormat);
                break;
            case TokenType::PrimaryKeyword:
                setFormat(token->position, token->length, primaryKeywordFormat);
                break;
            case TokenType::SecondaryKeyword:
                setFormat(token->position, token->length, secondaryKeywordFormat);
                break;
            case TokenType::String:
                setFormat(token->position, token->length, stringFormat);
                break;
            case TokenType::Integer:
            case TokenType::Float:
                setFormat(token->position, token->length, numberFormat);
                break;
            case TokenType::Variable:
                setFormat(token->position, token->length, variableFormat);
                break;
        }
    }
}
