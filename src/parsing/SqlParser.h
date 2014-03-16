#ifndef SQLPARSER_H
#define SQLPARSER_H

#include <QList>
#include <QString>
#include <string>

namespace parsing
{


class Token
{
public:
    enum Type
    {
        None,
        Space,
        SingleLineComment,
        Minus,
        LeftParen,
        RightParen,
        Semicolon,
        Plus,
        Star,
        Reminder,
        Equals,
        Comma,
        BitAnd,
        BitNot,
        Slash,
        MultiLineCommentStart,
        MultiLineComment,
        MultiLineCommentEnd,
        EndOfLine,
    };

    Token(Type type, int position, int length);

    Type type;
    int position;
    int length;
};

extern const unsigned char charType[];

class SqlParser
{
public:
    SqlParser();
    ~SqlParser();

    void setQueryText(const QString &newQueryText);
    void setLastTokenType(int tokenType);
    int getLastTokenType();

    QList<Token*> tokenList;

private:
    void clearTokenList();

    bool isSpace(char ch)
        { return charType[static_cast<unsigned char>(ch)] & 0x01; }

    bool isIdentifierChar(char ch)
        { return charType[static_cast<unsigned char>(ch)] & 0x46; }

    bool isSingleLineCommentEnd(char ch)
        { return ch == '\n' || ch == '\0'; }

    Token* getNewToken(Token::Type type, int length = 1)
        { return new Token(type, index, length); }

    void createTokenList();

    Token* getNextToken();
    Token* getSpaceToken();
    Token* parseMinus();
    Token* getSingleLineCommentToken();
    Token* getEqualsToken();
    Token* parseSlash();
    Token* getMultiLineCommentToken();

    QByteArray queryText;
    int index;
    Token::Type currentTokenType;
    Token *currentToken;

};

} // namespace parsing

#endif // SQLPARSER_H
