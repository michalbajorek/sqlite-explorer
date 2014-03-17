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

void SqlParser::setTextAndLastTokenType(const QString &newText, TokenType tokenType)
{
    initParser();
    text = newText;
    setLastTokenType(tokenType);
    createTokenList();
}

void SqlParser::clearTokenList()
{
    foreach(Token *token, tokenList)
        delete token;
    tokenList.clear();
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
            case ' ': case '\t': case '\n': case '\f': case '\r':
                parseSpaces();
                break;
            case '-':
                parseMinus();
                break;
            case '(':
                parseLeftParen();
                break;
            case ')':
                parseRightParen();
                break;
            case ';':
                parseSemicolon();
                break;
            case '+':
                parsePlus();
                break;
            case '*':
                parseStar();
                break;
            case '%':
                parseReminder();
                break;
            case ',':
                parseComma();
                break;
            case '&':
                parseBitAnd();
                break;
            case '~':
                parseBitNot();
                break;
            case '=':
                parseEquals();
                break;
            case '/':
                parseSlash();
                break;
            case '!':
                parseNotEqual();
                break;
            case '|':
                parsePipe();
                break;
            case '\'':
                parseString();
                break;
            case '\0':
                parseEndOfLine();
                break;
            default:
                if(keywords.isIdentifierChar(getChar()))
                    parseIdentifier();
                else
                    setNewCurrentToken(TokenType::Illegal);
                break;
        }
    }
}

void SqlParser::parseSpaces()
{
    int currentIndex = index + 1;
    while(isspace(text[currentIndex].unicode()))
        currentIndex ++;
    setNewCurrentToken(TokenType::Space, currentIndex - index);
}

void SqlParser::parseSingleLineComment()
{
    int currentIndex = index + 2;
    while(isSingleLineCommentEnd(text[currentIndex].unicode()) == false)
        currentIndex ++;
    setNewCurrentToken(TokenType::SingleLineComment, currentIndex - index);
}

void SqlParser::parseMultiLineComment()
{
    int currentIndex = index;
    while(text[currentIndex] != '\0' && (text[currentIndex] != '*' || text[currentIndex + 1] != '/'))
        currentIndex ++;
    if(text[currentIndex] == '\0')
        setNewCurrentToken(TokenType::MultiLineComment, currentIndex - index + 1);
    else
        setNewCurrentToken(TokenType::MultiLineCommentEnd, currentIndex - index + 2);
}

void SqlParser::parseMinus()
{
    if(text[index + 1] == '-')
        parseSingleLineComment();
    else
        setNewCurrentToken(TokenType::Minus);
}

void SqlParser::parseEquals()
{
    if(text[index + 1] == '=')
        setNewCurrentToken(TokenType::Equals, 2);
    else
        setNewCurrentToken(TokenType::Equals);
}

void SqlParser::parseNotEqual()
{
    if(text[index + 1] == '=')
        setNewCurrentToken(TokenType::NotEqual, 2);
    else
        setNewCurrentToken(TokenType::Illegal);
}

void SqlParser::parseSlash()
{
    if(text[index + 1] == '*')
        setNewCurrentToken(TokenType::MultiLineCommentStart, 2);
    else
        setNewCurrentToken(TokenType::Slash);
}

void SqlParser::parsePipe()
{
    if(text[index + 1] == '|')
        setNewCurrentToken(TokenType::Concat, 2);
    else
        setNewCurrentToken(TokenType::BitOr);
}

void SqlParser::parseIdentifier()
{

    int currentIndex = index + 1;
    while(keywords.isIdentifierChar(text[currentIndex].unicode()))
        currentIndex ++;
    QString identifier = text.mid(index, currentIndex - index).toUpper();
    if(keywords.isPrimaryKeyword(identifier))
        setNewCurrentToken(TokenType::PrimaryKeyword, currentIndex - index);
    else if(keywords.isSecondaryKeyword(identifier))
        setNewCurrentToken(TokenType::SecondaryKeyword, currentIndex - index);
    else
        setNewCurrentToken(TokenType::Identifier, currentIndex - index);
}

void SqlParser::parseString()
{
    int currentIndex = 1;
    while(getChar(currentIndex) != L'\'' && getChar(currentIndex) != '\0')
        currentIndex ++;
    setNewCurrentToken(TokenType::String, currentIndex + 1);
}


/*//            case '`': case '\'': case '"': {
//      int delim = z[0];
//      for(i=1; (c=z[i])!=0; i++){
//        if( c==delim ){
//          if( z[i+1]==delim ){
//            i++;
//          }else{
//            break;
//          }
//        }
//      }
//      if( c=='\'' ){
//        *tokenType = TK_STRING;
//        return i+1;
//      }else if( c!=0 ){
//        *tokenType = TK_ID;
//        return i+1;
//      }else{
//        *tokenType = TK_ILLEGAL;
//        return i;
//      }
//    }*/


/*
//
// Return the length of the token that begins at z[0].
// Store the token type in *tokenType before returning.
//
int sqlite3GetToken(const unsigned char *z, int *tokenType){
  int i, c;
  switch( *z ){


    case '<': {
      if( (c=z[1])=='=' ){
        *tokenType = TK_LE;
        return 2;
      }else if( c=='>' ){
        *tokenType = TK_NE;
        return 2;
      }else if( c=='<' ){
        *tokenType = TK_LSHIFT;
        return 2;
      }else{
        *tokenType = TK_LT;
        return 1;
      }
    }
    case '>': {
      if( (c=z[1])=='=' ){
        *tokenType = TK_GE;
        return 2;
      }else if( c=='>' ){
        *tokenType = TK_RSHIFT;
        return 2;
      }else{
        *tokenType = TK_GT;
        return 1;
      }
    }

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
    case '.': {
#ifndef SQLITE_OMIT_FLOATING_POINT
      if( !sqlite3Isdigit(z[1]) )
#endif
      {
        *tokenType = TK_DOT;
        return 1;
      }
      // If the next character is a digit, this is a floating point
      // number that begins with ".".  Fall thru into the next case
    }
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9': {
      testcase( z[0]=='0' );  testcase( z[0]=='1' );  testcase( z[0]=='2' );
      testcase( z[0]=='3' );  testcase( z[0]=='4' );  testcase( z[0]=='5' );
      testcase( z[0]=='6' );  testcase( z[0]=='7' );  testcase( z[0]=='8' );
      testcase( z[0]=='9' );
      *tokenType = TK_INTEGER;
      for(i=0; sqlite3Isdigit(z[i]); i++){}
#ifndef SQLITE_OMIT_FLOATING_POINT
      if( z[i]=='.' ){
        i++;
        while( sqlite3Isdigit(z[i]) ){ i++; }
        *tokenType = TK_FLOAT;
      }
      if( (z[i]=='e' || z[i]=='E') &&
           ( sqlite3Isdigit(z[i+1])
            || ((z[i+1]=='+' || z[i+1]=='-') && sqlite3Isdigit(z[i+2]))
           )
      ){
        i += 2;
        while( sqlite3Isdigit(z[i]) ){ i++; }
        *tokenType = TK_FLOAT;
      }
#endif
      while( IdChar(z[i]) ){
        *tokenType = TK_ILLEGAL;
        i++;
      }
      return i;
    }
    case '[': {
      for(i=1, c=z[0]; c!=']' && (c=z[i])!=0; i++){}
      *tokenType = c==']' ? TK_ID : TK_ILLEGAL;
      return i;
    }
    case '?': {
      *tokenType = TK_VARIABLE;
      for(i=1; sqlite3Isdigit(z[i]); i++){}
      return i;
    }
#ifndef SQLITE_OMIT_TCL_VARIABLE
    case '$':
#endif
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
#ifndef SQLITE_OMIT_TCL_VARIABLE
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
#endif
        }else{
          break;
        }
      }
      if( n==0 ) *tokenType = TK_ILLEGAL;
      return i;
    }
#ifndef SQLITE_OMIT_BLOB_LITERAL
    case 'x': case 'X': {
      testcase( z[0]=='x' ); testcase( z[0]=='X' );
      if( z[1]=='\'' ){
        *tokenType = TK_BLOB;
        for(i=2; sqlite3Isxdigit(z[i]); i++){}
        if( z[i]!='\'' || i%2 ){
          *tokenType = TK_ILLEGAL;
          while( z[i] && z[i]!='\'' ){ i++; }
        }
        if( z[i] ) i++;
        return i;
      }
      // Otherwise fall through to the next case
    }
#endif
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
