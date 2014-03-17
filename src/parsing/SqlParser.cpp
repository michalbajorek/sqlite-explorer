#include <QDebug>
#include "SqlParser.h"
#include "../common/Exception.h"

using namespace parsing;

const unsigned char parsing::charType[256] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 00..07    ........
  0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,  // 08..0f    ........
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 10..17    ........
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 18..1f    ........
  0x01, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,  // 20..27     !"#$%&'
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 28..2f    ()*+,-./
  0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c,  // 30..37    01234567
  0x0c, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 38..3f    89:;<=>?

  0x00, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x02,  // 40..47    @ABCDEFG
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,  // 48..4f    HIJKLMNO
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,  // 50..57    PQRSTUVW
  0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x40,  // 58..5f    XYZ[\]^_
  0x00, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x22,  // 60..67    `abcdefg
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,  // 68..6f    hijklmno
  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,  // 70..77    pqrstuvw
  0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00,  // 78..7f    xyz{|}~.

  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // 80..87    ........
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // 88..8f    ........
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // 90..97    ........
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // 98..9f    ........
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // a0..a7    ........
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // a8..af    ........
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // b0..b7    ........
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // b8..bf    ........

  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // c0..c7    ........
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // c8..cf    ........
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // d0..d7    ........
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // d8..df    ........
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // e0..e7    ........
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // e8..ef    ........
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,  // f0..f7    ........
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40   // f8..ff    ........
};

enum
{
    KeywordsCount = 105,
    MainKeywordsCount = 19,
};

static_assert(KeywordsCount + MainKeywordsCount == 124, "Invalid keywords count");

const char *mainKeywords[MainKeywordsCount] =
{
    "ALTER",
    "ANALYZE",
    "ATTACH",
    "EXPLAIN",
    "BEGIN",
    "COMMIT",
    "CREATE",
    "DELETE",
    "DETACH",
    "DROP",
    "INSERT",
    "PRAGMA",
    "REINDEX",
    "RELEASE",
    "ROLLBACK",
    "SAVEPOINT",
    "SELECT",
    "UPDATE",
    "VACUUM",
};

const char *keywords[KeywordsCount] =
{
    "ABORT",
    "ACTION",
    "ADD",
    "AFTER",
    "ALL",
    "AND",
    "AS",
    "ASC",
    "AUTOINCREMENT",
    "BEFORE",
    "BETWEEN",
    "BY",
    "CASCADE",
    "CASE",
    "CAST",
    "CHECK",
    "COLLATE",
    "COLUMN",
    "CONFLICT",
    "CONSTRAINT",
    "CROSS",
    "CURRENT_DATE",
    "CURRENT_TIME",
    "CURRENT_TIMESTAMP",
    "DATABASE",
    "DEFAULT",
    "DEFERRABLE",
    "DEFERRED",
    "DESC",
    "DISTINCT",
    "EACH",
    "ELSE",
    "END",
    "ESCAPE",
    "EXCEPT",
    "EXCLUSIVE",
    "EXISTS",
    "FAIL",
    "FOR",
    "FOREIGN",
    "FROM",
    "FULL",
    "GLOB",
    "GROUP",
    "HAVING",
    "IF",
    "IGNORE",
    "IMMEDIATE",
    "IN",
    "INDEX",
    "INDEXED",
    "INITIALLY",
    "INNER",
    "INSTEAD",
    "INTERSECT",
    "INTO",
    "IS",
    "ISNULL",
    "JOIN",
    "KEY",
    "LEFT",
    "LIKE",
    "LIMIT",
    "MATCH",
    "NATURAL",
    "NO",
    "NOT",
    "NOTNULL",
    "NULL",
    "OF",
    "OFFSET",
    "ON",
    "OR",
    "ORDER",
    "OUTER",
    "PLAN",
    "PRIMARY",
    "QUERY",
    "RAISE",
    "RECURSIVE",
    "REFERENCES",
    "REGEXP",
    "RENAME",
    "REPLACE",
    "RESTRICT",
    "RIGHT",
    "ROW",
    "SET",
    "TABLE",
    "TEMP",
    "TEMPORARY",
    "THEN",
    "TO",
    "TRANSACTION",
    "TRIGGER",
    "UNION",
    "UNIQUE",
    "USING",
    "VALUES",
    "VIEW",
    "VIRTUAL",
    "WHEN",
    "WHERE",
    "WITH",
    "WITHOUT",
};

QSet<QString> SqlParser::mainKeywordSet;
QSet<QString> SqlParser::keywordSet;

Token::Token(Type type, int position, int length)
{
    this->type = type;
    this->position = position;
    this->length = length;
}

SqlParser::SqlParser()
{
    currentToken = nullptr;
    currentTokenType = Token::None;
    createKeywordSets();
}

SqlParser::~SqlParser()
{
    clearTokenList();
}

void SqlParser::createKeywordSets()
{
    if(keywordSet.isEmpty())
    {
        for(int i = 0; i < KeywordsCount; i++)
            keywordSet.insert(QString(keywords[i]));
    }

    if(mainKeywordSet.isEmpty())
    {
        for(int i = 0; i < MainKeywordsCount; i++)
            mainKeywordSet.insert(QString(mainKeywords[i]));
    }
}

void SqlParser::setQueryText(const QString &newQueryText)
{
    queryText = newQueryText;
    index = 0;
    createTokenList();
}

void SqlParser::setLastTokenType(int tokenType)
{
    if(tokenType == -1)
        currentTokenType = Token::None;
    else
        currentTokenType = Token::Type(tokenType);
}

int SqlParser::getLastTokenType()
{
    return currentTokenType;
}

void SqlParser::clearTokenList()
{
    foreach(Token *token, tokenList)
        delete token;
    tokenList.clear();
}

bool SqlParser::isMainKeyword(const QString &identifier)
{
    return mainKeywordSet.contains(identifier);
}

bool SqlParser::isKeyword(const QString &identifier)
{
    return keywordSet.contains(identifier);
}

void SqlParser::createTokenList()
{
    clearTokenList();
    while(Token *token = getNextToken())
    {
        tokenList.append(token);
        if(token->type == Token::EndOfLine || token->type == Token::MultiLineComment)
            break;
    }
}

Token *SqlParser::getNextToken()
{
    currentToken = nullptr;
    if(currentTokenType == Token::MultiLineCommentStart || currentTokenType == Token::MultiLineComment)
        parseMultiLineComment();
    else
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
                if(isIdentifierChar(getChar()))
                    parseIdentifier();
                else
                    setNewCurrentToken(Token::Illegal);
                break;
        }

    if(currentToken == nullptr)
        throw common::Exception("Token can not be null");

    index += currentToken->length;
    currentTokenType = currentToken->type;
    return currentToken;
}

void SqlParser::parseSpaces()
{
    int currentIndex = index + 1;
    while(isSpace(queryText[currentIndex].unicode()))
        currentIndex ++;
    setNewCurrentToken(Token::Space, currentIndex - index);
}

void SqlParser::parseSingleLineComment()
{
    int currentIndex = index + 2;
    while(isSingleLineCommentEnd(queryText[currentIndex].unicode()) == false)
        currentIndex ++;
    setNewCurrentToken(Token::SingleLineComment, currentIndex - index);
}

void SqlParser::parseMultiLineComment()
{
    int currentIndex = index;
    while(queryText[currentIndex] != '\0' && (queryText[currentIndex] != '*' || queryText[currentIndex + 1] != '/'))
        currentIndex ++;
    if(queryText[currentIndex] == '\0')
        setNewCurrentToken(Token::MultiLineComment, currentIndex - index + 1);
    else
        setNewCurrentToken(Token::MultiLineCommentEnd, currentIndex - index + 2);
}

void SqlParser::parseMinus()
{
    if(queryText[index + 1] == '-')
        parseSingleLineComment();
    else
        setNewCurrentToken(Token::Minus);
}

void SqlParser::parseEquals()
{
    if(queryText[index + 1] == '=')
        setNewCurrentToken(Token::Equals, 2);
    else
        setNewCurrentToken(Token::Equals);
}

void SqlParser::parseNotEqual()
{
    if(queryText[index + 1] == '=')
        setNewCurrentToken(Token::NotEqual, 2);
    else
        setNewCurrentToken(Token::Illegal);
}

void SqlParser::parseSlash()
{
    if(queryText[index + 1] == '*')
        setNewCurrentToken(Token::MultiLineCommentStart, 2);
    else
        setNewCurrentToken(Token::Slash);
}

void SqlParser::parsePipe()
{
    if(queryText[index + 1] == '|')
        setNewCurrentToken(Token::Concat, 2);
    else
        setNewCurrentToken(Token::BitOr);
}

void SqlParser::parseIdentifier()
{

    int currentIndex = index + 1;
    while(isIdentifierChar(queryText[currentIndex].unicode()))
        currentIndex ++;
    QString identifier = queryText.mid(index, currentIndex - index).toUpper();
    if(isMainKeyword(identifier))
        setNewCurrentToken(Token::MainKeyword, currentIndex - index);
    else if(isKeyword(identifier))
        setNewCurrentToken(Token::Keyword, currentIndex - index);
    else
        setNewCurrentToken(Token::Identifier, currentIndex - index);
}

void SqlParser::parseString()
{
    int currentIndex = 1;
    while(getChar(currentIndex) != L'\'' && getChar(currentIndex) != '\0')
        currentIndex ++;
    setNewCurrentToken(Token::String, currentIndex + 1);
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
