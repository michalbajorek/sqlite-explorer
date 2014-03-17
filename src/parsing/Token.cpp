#include "Token.h"

using namespace parsing;

Token::Token(TokenType type, int position, int length)
{
    this->type = type;
    this->position = position;
    this->length = length;
}
