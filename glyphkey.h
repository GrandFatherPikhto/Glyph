#ifndef GLYPHKEY_H
#define GLYPHKEY_H

#include <QFont>
#include <QString>
#include <QChar>
#include <QDataStream>
#include <QHash>
#include <QDebug>

class GlyphKey {
public:
    GlyphKey(int character /* = -1 */, int gridSize /* = -1 */)
    : m_character(character)
    , m_gridSize(gridSize)
    {

    }

    GlyphKey(const QChar &character, int gridSize)
        : m_character(character.unicode())
        , m_gridSize(gridSize)
    {
    
    }

    // Оператор < для QMap
    bool operator < (const GlyphKey &other) const {
        if (m_character != other.m_character)
            return m_character < other.m_character;
        return m_gridSize < other.m_gridSize;
    }

    // Оператор == для QHash
    bool operator==(const GlyphKey &other) const {
        return m_character == other.m_character &&
               m_gridSize == other.m_gridSize;
    }

    // Сеттеры
    void setCharacter(int newCharacter) {m_character = newCharacter;}
    void setGridSize(int newGridSize) { m_gridSize = newGridSize; }

    // Геттеры
    int character() const { return m_character; }
    int gridSize() const { return m_gridSize; }


private:
    int m_character;
    int m_gridSize;
};

// inline uint qHash(const GlyphKey &key, uint seed = 0);
inline uint qHash(const GlyphKey &key, uint seed) {
    QtPrivate::QHashCombine hash;
    seed = hash(seed, key.character());
    seed = hash(seed, key.gridSize());
    return seed;
}


#endif // GLYPHKEY_H
