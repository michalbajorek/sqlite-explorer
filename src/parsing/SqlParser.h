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
        String,
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

    void setNewCurrentToken(Token::Type type, int length = 1)
        { currentToken = new Token(type, index, length); }

    wchar_t getChar(int offset = 0) const
        {
        offset += index;
        return offset < queryText.length() ? queryText[offset].unicode() : 0;
        }

    bool isMainKeyword(const QString &identifier);
    bool isKeyword(const QString &identifier);

    void createTokenList();

    Token *getNextToken();

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

    void parseLeftParen()
        { setNewCurrentToken(Token::LeftParen); }

    void parseRightParen()
        { setNewCurrentToken(Token::RightParen); }

    void parseSemicolon()
        { setNewCurrentToken(Token::Semicolon); }

    void parsePlus()
        { setNewCurrentToken(Token::Plus); }

    void parseStar()
        { setNewCurrentToken(Token::Star); }

    void parseReminder()
        { setNewCurrentToken(Token::Reminder); }

    void parseComma()
        { setNewCurrentToken(Token::Comma); }

    void parseBitAnd()
        { setNewCurrentToken(Token::BitAnd); }

    void parseBitNot()
        { setNewCurrentToken(Token::BitNot); }

    void parseEndOfLine()
        { setNewCurrentToken(Token::EndOfLine); }

    QString queryText;
    int index;
    Token::Type currentTokenType;
    Token *currentToken;

    static QSet<QString> mainKeywordSet;
    static QSet<QString> keywordSet;

};

} // namespace parsing

#endif // SQLPARSER_H
