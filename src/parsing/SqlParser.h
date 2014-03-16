#ifndef SQLPARSER_H
#define SQLPARSER_H

#include <QList>
#include <QSet>
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
        NotEqual,
        Illegal,
        Concat,
        BitOr,
        Identifier,
        MainKeyword,
        Keyword,
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

    void createKeywordSets();
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

    bool isMainKeyword(const QString &identifier);
    bool isKeyword(const QString &identifier);

    void createTokenList();

    Token* getNextToken();
    Token* getSpaceToken();
    Token* parseMinus();
    Token* getSingleLineCommentToken();
    Token* parseEquals();
    Token* parseSlash();
    Token* getMultiLineCommentToken();
    Token* parseNotEqual();
    Token* parsePipe();
    Token* parseIdentifier();

    QByteArray queryText;
    int index;
    Token::Type currentTokenType;
    Token *currentToken;

    static QSet<QString> mainKeywordSet;
    static QSet<QString> keywordSet;

};

} // namespace parsing

#endif // SQLPARSER_H
