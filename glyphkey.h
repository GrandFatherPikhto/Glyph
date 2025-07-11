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
    GlyphKey(const QFont &font, const QChar &character, int gridSize)
        : m_font(font), m_character(character), m_gridSize(gridSize) {}
    GlyphKey() : m_font(QFont()), m_gridSize(-1), m_character(QChar()) {}
    GlyphKey(const GlyphKey &newKey)
        : m_font(newKey.m_font)
        , m_character(newKey.m_character)
        , m_gridSize(newKey.m_gridSize)
    {

    }

    // Оператор < для QMap
    bool operator < (const GlyphKey &other) const {
        if (m_font != other.m_font)
            return m_font.family() < other.m_font.family();
        if (m_character != other.m_character)
            return m_character.unicode() < other.m_character.unicode();
        return m_gridSize < other.m_gridSize;
    }

    // Оператор == для QHash
    bool operator==(const GlyphKey &other) const {
        return m_font == other.m_font &&
               m_character == other.m_character &&
               m_gridSize == other.m_gridSize;
    }

    // Сеттеры
    void setFont(const QFont &newFont) { m_font = newFont; }
    void setCharacter(const QChar &newCharacter) {m_character = newCharacter;}
    void setGridSize(int newGridSize) { m_gridSize = newGridSize; }

    // Геттеры
    const QFont& font() const { return m_font; }
    const QChar& character() const { return m_character; }
    int gridSize() const { return m_gridSize; }

    void printValidation() {
        QString validation((m_font != QFont() && m_character != QChar() && m_gridSize > 0) ? "Valid" : "Not Valid");
        qDebug() << "GlyphKey: " << validation << m_font.family() << m_character << m_gridSize;
    }

    bool isValid() {
        return (m_font != QFont() && m_character != QChar() && m_gridSize > 0);
    }

    // Сериализация
    friend QDataStream &operator<<(QDataStream &out, const GlyphKey &key) {
        out << key.m_font.family() << key.m_character << key.m_gridSize;
        return out;
    }

    // Десериализация
    friend QDataStream &operator>>(QDataStream &in, GlyphKey &key) {
        QString fontFamily;
        QChar character;
        int gridSize;
        in >> fontFamily >> character >> gridSize;
        key.m_font = QFont(fontFamily);
        key.m_character = character;
        key.m_gridSize = gridSize;
        return in;
    }

private:
    QFont m_font;
    QChar m_character;
    int m_gridSize;
};

// inline uint qHash(const GlyphKey &key, uint seed = 0);
inline uint qHash(const GlyphKey &key, uint seed) {
    QtPrivate::QHashCombine hash;
    seed = hash(seed, qHash(key.font().family()));
    seed = hash(seed, key.character().unicode());
    seed = hash(seed, key.gridSize());
    return seed;
}


#endif // GLYPHKEY_H
