#ifndef TOKEN_H
#define TOKEN_H

namespace parsing
{

enum class TokenType
{
    None = -1,
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
    PrimaryKeyword,
    SecondaryKeyword,
    String,
};

class Token
{
public:
    Token(TokenType type, int position, int length);

    TokenType type;
    int position;
    int length;
};

} // namespace parsing

#endif // TOKEN_H
