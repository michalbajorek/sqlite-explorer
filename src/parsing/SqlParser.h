#ifndef SQLPARSER_H
#define SQLPARSER_H

#include <QDebug>

#include <QList>
#include <QSet>
#include <QString>
#include <string>

#include "Keywords.h"
#include "Token.h"

namespace parsing
{

extern const unsigned char charType[];

class SqlParser
{
public:
    SqlParser();
    ~SqlParser();

    void setTextAndLastTokenType(const QString &newText, TokenType tokenType);


    TokenType getLastTokenType() const
        { return lastTokenType; }

    QList<Token*> tokenList;

private:
    void initParser();
    void clearTokenList();

    void setLastTokenType(TokenType tokenType)
        { lastTokenType = tokenType; }

    void advanceCurrentIndex()
        { index += currentToken->length; }

    bool isEndOfLine()
        { return currentToken->type == TokenType::EndOfLine; }

    bool isSingleLineCommentEnd(char ch)
        { return ch == '\n' || ch == '\0'; }

    void setNewCurrentToken(TokenType type, int length = 1)
    {
        qDebug() << "Token: " << int(type) << " Index: " << index << " Length: " << length;
        currentToken = new Token(type, index, length);
    }

    wchar_t getChar(int offset = 0) const
        {
        offset += index;
        return offset < text.length() ? text[offset].unicode() : 0;
        }

    void createTokenList();
    void addNextToken();
    void parseToken();
    void parseSpaces();
    void parseSingleLineComment();
    void parseMultiLineComment();
    void parseMinus();
    void parseEquals();
    void parseSlash();
    void parseNotEqual();
    void parsePipe();
    void parseIdentifier();
    void parseString();
    void parseLessThan();
    void parseGraterThan();
    void parseDot();
    void parseNumber();
    void parseSquareBrackets();
    void parseQuestionMark();

    void parseLeftParen()
        { setNewCurrentToken(TokenType::LeftParen); }

    void parseRightParen()
        { setNewCurrentToken(TokenType::RightParen); }

    void parseSemicolon()
        { setNewCurrentToken(TokenType::Semicolon); }

    void parsePlus()
        { setNewCurrentToken(TokenType::Plus); }

    void parseStar()
        { setNewCurrentToken(TokenType::Star); }

    void parseReminder()
        { setNewCurrentToken(TokenType::Reminder); }

    void parseComma()
        { setNewCurrentToken(TokenType::Comma); }

    void parseBitAnd()
        { setNewCurrentToken(TokenType::BitAnd); }

    void parseBitNot()
        { setNewCurrentToken(TokenType::BitNot); }

    void parseEndOfLine()
        { setNewCurrentToken(TokenType::EndOfLine); }

    void skipDigits(int &currentIndex);

    QString text;
    int index;
    TokenType lastTokenType;
    Token *currentToken;
    Keywords keywords;
};

} // namespace parsing

#endif // SQLPARSER_H
