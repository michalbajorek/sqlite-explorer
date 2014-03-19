#include <QDebug>
#include "SqlParser.h"
#include "../common/Exception.h"

using namespace parsing;


SqlParser::SqlParser()
{
    initParser();
}

SqlParser::~SqlParser()
{
    clearTokenList();
}

void SqlParser::initParser()
{
    index = 0;
    currentToken = nullptr;
    lastTokenType = TokenType::None;
}

void SqlParser::clearTokenList()
{
    foreach(Token *token, tokenList)
        delete token;
    tokenList.clear();
}

void SqlParser::setTextAndLastTokenType(const QString &newText, TokenType tokenType)
{
    //qDebug() << "Sql: " << newText;
    initParser();
    text = newText;
    setLastTokenType(tokenType);
    createTokenList();
}

void SqlParser::createTokenList()
{
    clearTokenList();
    while(true)
    {
        addNextToken();
        advanceCurrentIndex();
        if(isEndOfLine())
            break;
        setLastTokenType(currentToken->type);
    }
}

void SqlParser::addNextToken()
{
    parseToken();
    if(currentToken == nullptr)
        throw common::Exception("Undefined syntax error");
    tokenList.append(currentToken);
}

void SqlParser::parseToken()
{
    if(lastTokenType == TokenType::MultiLineCommentStart || lastTokenType == TokenType::MultiLineComment)
    {
        if(getChar() == '\0')
            parseEndOfLine();
        else
            parseMultiLineComment();
    }
    else
    {
        switch(getChar())
        {
            case L' ': case L'\t': case L'\n': case L'\f': case L'\r':
                parseSpaces();
                break;
            case L'-':
                parseMinus();
                break;
            case L'(':
                parseLeftParen();
                break;
            case L')':
                parseRightParen();
                break;
            case L';':
                parseSemicolon();
                break;
            case L'+':
                parsePlus();
                break;
            case L'*':
                parseStar();
                break;
            case L'%':
                parseReminder();
                break;
            case L',':
                parseComma();
                break;
            case L'&':
                parseBitAnd();
                break;
            case L'~':
                parseBitNot();
                break;
            case L'=':
                parseEquals();
                break;
            case L'/':
                parseSlash();
                break;
            case L'!':
                parseNotEqual();
                break;
            case L'|':
                parsePipe();
                break;
            case L'<':
                parseLessThan();
                break;
            case L'>':
                parseGraterThan();
                break;
            case L'\'':
                parseString();
                break;
            case L'.':
                parseDot();
                break;
            case L'0': case L'1': case L'2': case L'3': case L'4': case L'5': case L'6': case L'7': case L'8': case L'9':
                parseNumber();
                break;
            case L'[':
                parseSquareBrackets();
                break;
            case L'?':
                parseQuestionMark();
                break;
            case L'\0':
                parseEndOfLine();
                break;
            case L'x': case L'X':
                parseBlobOrIdentifier();
                break;
            default:
                parseIdentifier();
                break;
        }
    }
}

void SqlParser::parseMultiLineComment()
{
    int tokenLength = 0;
    while(getChar(tokenLength) != L'\0' && (getChar(tokenLength) != L'*' || getChar(tokenLength + 1) != L'/'))
        tokenLength ++;
    if(getChar(tokenLength) == '\0')
        setNewCurrentToken(TokenType::MultiLineComment, tokenLength);
    else
        setNewCurrentToken(TokenType::MultiLineCommentEnd, tokenLength + 2);
}

void SqlParser::parseSpaces()
{
    int currentIndex = 1;
    skipCharsThatMeetCriteria(currentIndex, Keywords::isSpaceChar);
    setNewCurrentToken(TokenType::Space, currentIndex);
}

void SqlParser::parseSingleLineComment()
{
    int currentIndex = 2;
    while(isSingleLineCommentEnd(getChar(currentIndex)) == false)
        currentIndex ++;
    setNewCurrentToken(TokenType::SingleLineComment, currentIndex);
}

void SqlParser::parseMinus()
{
    if(getChar(1) == '-')
        parseSingleLineComment();
    else
        setNewCurrentToken(TokenType::Minus);
}

void SqlParser::parseEquals()
{
    if(getChar(1) == '=')
        setNewCurrentToken(TokenType::Equals, 2);
    else
        setNewCurrentToken(TokenType::Equals);
}

void SqlParser::parseNotEqual()
{
    if(getChar(1) == '=')
        setNewCurrentToken(TokenType::NotEqual, 2);
    else
        setNewCurrentToken(TokenType::Illegal);
}

void SqlParser::parseSlash()
{
    if(getChar(1) == '*')
        setNewCurrentToken(TokenType::MultiLineCommentStart, 2);
    else
        setNewCurrentToken(TokenType::Slash);
}

void SqlParser::parsePipe()
{
    if(getChar(1) == '|')
        setNewCurrentToken(TokenType::Concat, 2);
    else
        setNewCurrentToken(TokenType::BitOr);
}

void SqlParser::parseIdentifier()
{
    int currentIndex = 0;
    skipCharsThatMeetCriteria(currentIndex, Keywords::isIdentifierChar);
    if(currentIndex == 0)
        setNewCurrentToken(TokenType::Illegal);
    else
        {
        QString identifier = text.mid(index, currentIndex).toUpper();
        if(keywords.isPrimaryKeyword(identifier))
            setNewCurrentToken(TokenType::PrimaryKeyword, currentIndex);
        else if(keywords.isSecondaryKeyword(identifier))
            setNewCurrentToken(TokenType::SecondaryKeyword, currentIndex);
        else
            setNewCurrentToken(TokenType::Identifier, currentIndex);
        }
}

void SqlParser::parseString()
{
    int currentIndex = 1;
    while(true)
    {
        if(getChar(currentIndex) == L'\0')
            break;
        if(getChar(currentIndex) == L'\'')
        {
            currentIndex ++;
            if(getChar(currentIndex) != L'\'') // Double apostrophe does not divide string token
                break;
        }
        currentIndex ++;
    }
    setNewCurrentToken(TokenType::String, currentIndex);
}

void SqlParser::parseLessThan()
{
    wchar_t character = getChar(1);
    if(character == L'=')
        setNewCurrentToken(TokenType::LessOrEqual, 2);
    else if(character == L'>')
        setNewCurrentToken(TokenType::NotEqual, 2);
    else if(character == L'<')
        setNewCurrentToken(TokenType::LeftShift, 2);
    else
        setNewCurrentToken(TokenType::LessThan);
}

void SqlParser::parseGraterThan()
{
    wchar_t character = getChar(1);
    if(character == L'=')
        setNewCurrentToken(TokenType::GreaterEqual, 2);
    else if(character == L'>')
        setNewCurrentToken(TokenType::RightShift, 2);
    else
        setNewCurrentToken(TokenType::GreaterThan);
}

void SqlParser::parseDot()
{
    if(Keywords::isDigitChar(getChar(1)))
        parseNumber();
    else
        setNewCurrentToken(TokenType::Dot);
}

void SqlParser::parseNumber()
{
    TokenType tokenType = TokenType::Integer;
    int currentIndex = 1;
    skipCharsThatMeetCriteria(currentIndex, Keywords::isDigitChar);
    // Check floating point
    if(getChar(currentIndex) == L'.')
    {
        currentIndex ++;
        skipCharsThatMeetCriteria(currentIndex, Keywords::isDigitChar);
        tokenType = TokenType::Float;
    }
    // Check scientific notation
    wchar_t character = getChar(currentIndex);
    if(character == L'e' || character == L'E')
    {
        currentIndex ++;
        character = getChar(currentIndex);
        if(Keywords::isDigitChar(character) || character == L'+' || character == L'-')
            {
            currentIndex ++;
            if(Keywords::isDigitChar(getChar(currentIndex)))
                skipCharsThatMeetCriteria(currentIndex, Keywords::isDigitChar);
            }
    }
    while(Keywords::isIdentifierChar(getChar(currentIndex)))
    {
        tokenType = TokenType::Illegal;
        currentIndex ++;
    }
    setNewCurrentToken(tokenType, currentIndex);
}

void SqlParser::skipCharsThatMeetCriteria(int &currentIndex, CriteriaFunction function)
{
    while(function(getChar(currentIndex)))
        currentIndex ++;
}

void SqlParser::parseSquareBrackets()
{
    TokenType tokenType = TokenType::Illegal;
    int currentIndex = 1;
    while(true)
    {
        wchar_t character = getChar(currentIndex++);
        if(character == L'\0')
            break;
        if(character == L']')
        {
            tokenType = TokenType::Identifier;
            break;
        }
    }
    setNewCurrentToken(tokenType, currentIndex);
}

void SqlParser::parseQuestionMark()
{
    int currentIndex = 1;
    skipCharsThatMeetCriteria(currentIndex, Keywords::isDigitChar);
    setNewCurrentToken(TokenType::Variable, currentIndex);
}

void SqlParser::parseBlobOrIdentifier()
{
    if(getChar(1) == L'\'')
        parseBlob();
    else
        parseIdentifier();
}

void SqlParser::parseBlob()
{
    int currentIndex = 2;
    skipCharsThatMeetCriteria(currentIndex, Keywords::isHexDigitChar);
    if(getChar(currentIndex) == L'\'')
    {
        currentIndex ++;
        if(currentIndex % 2 == 0)
            setNewCurrentToken(TokenType::Illegal, currentIndex);
        else
            setNewCurrentToken(TokenType::Blob, currentIndex);
    }
    else
        setNewCurrentToken(TokenType::Blob, currentIndex);
}


/*
//
// Return the length of the token that begins at z[0].
// Store the token type in *tokenType before returning.
//
int sqlite3GetToken(const unsigned char *z, int *tokenType){
  int i, c;
  switch( *z ){

    case '`':
    case '\'':
    case '"': {
      int delim = z[0];
      testcase( delim=='`' );
      testcase( delim=='\'' );
      testcase( delim=='"' );
      for(i=1; (c=z[i])!=0; i++){
        if( c==delim ){
          if( z[i+1]==delim ){
            i++;
          }else{
            break;
          }
        }
      }
      if( c=='\'' ){
        *tokenType = TK_STRING;
        return i+1;
      }else if( c!=0 ){
        *tokenType = TK_ID;
        return i+1;
      }else{
        *tokenType = TK_ILLEGAL;
        return i;
      }
    }
    case '$':
    case '@':  //For compatibility with MS SQL Server
    case '#':
    case ':': {
      int n = 0;
      testcase( z[0]=='$' );  testcase( z[0]=='@' );
      testcase( z[0]==':' );  testcase( z[0]=='#' );
      *tokenType = TK_VARIABLE;
      for(i=1; (c=z[i])!=0; i++){
        if( IdChar(c) ){
          n++;
        }else if( c=='(' && n>0 ){
          do{
            i++;
          }while( (c=z[i])!=0 && !sqlite3Isspace(c) && c!=')' );
          if( c==')' ){
            i++;
          }else{
            *tokenType = TK_ILLEGAL;
          }
          break;
        }else if( c==':' && z[i+1]==':' ){
          i++;
        }else{
          break;
        }
      }
      if( n==0 ) *tokenType = TK_ILLEGAL;
      return i;
    }

    default: {
      if( !IdChar(*z) ){
        break;
      }
      for(i=1; IdChar(z[i]); i++){}
      *tokenType = keywordCode((char*)z, i);
      return i;
    }
  }
  *tokenType = TK_ILLEGAL;
  return 1;
}*/
