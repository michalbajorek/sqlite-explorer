#ifndef KEYWORDS_H
#define KEYWORDS_H

#include <QSet>
#include <QString>

namespace parsing
{

typedef bool (*CriteriaFunction)(char characted);

class Keywords
{
public:
    Keywords();

    bool isPrimaryKeyword(const QString &identifier)
        { return primaryKeywordSet.contains(identifier); }

    bool isSecondaryKeyword(const QString &identifier)
        { return secondaryKeywordSet.contains(identifier); }

    static bool isIdentifierChar(char character)
        { return charType[static_cast<unsigned char>(character)] & 0x46; } // from sqlite3.c

    static bool isDigitChar(char character)
        { return charType[static_cast<unsigned char>(character)] & 0x04; } // from sqlite3.c

    static bool isHexDigitChar(char character)
        { return charType[static_cast<unsigned char>(character)] & 0x08; } // from sqlite3.c

    static bool isSpaceChar(char character)
        { return charType[static_cast<unsigned char>(character)] & 0x01; } // from sqlite3.c

private:
    void createKeywordSets();
    void createPrimaryKeywordSet();
    void createSecondaryKeywordSet();

    static QSet<QString> primaryKeywordSet;
    static QSet<QString> secondaryKeywordSet;

    static const int SecondaryKeywordsCount = 105;
    static const int PrimaryKeywordsCount = 19;

    static const char *primaryKeywords[PrimaryKeywordsCount];
    static const char *secondaryKeywords[SecondaryKeywordsCount];
    static const unsigned char charType[256];

    // Keyword list available at http://www.sqlite.org/lang_keywords.html (total keyword count 124)
    static_assert(Keywords::SecondaryKeywordsCount + Keywords::PrimaryKeywordsCount == 124, "Invalid keywords count");
};

} // namespace parsing

#endif // KEYWORDS_H
