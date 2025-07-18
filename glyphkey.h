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
    GlyphKey(int character /* = -1 */, int bitmapDimension /* = -1 */)
    : m_unicode(character)
    , m_bitmapDimension(bitmapDimension)
    {

    }

    GlyphKey(const QChar &character, int bitmapDimension, const QFont &font)
        : m_unicode(character.unicode())
        , m_bitmapDimension(bitmapDimension)
    {
    
    }

    GlyphKey(const GlyphKey &key)
        : m_unicode(key.m_unicode)
        , m_bitmapDimension(key.m_bitmapDimension)
    {
    
    }

    // Оператор < для QMap
    bool operator < (const GlyphKey &other) const {
        if (m_unicode != other.m_unicode)
            return m_unicode < other.m_unicode;
        return m_bitmapDimension < other.m_bitmapDimension;
    }

    QString toString() const {
        return QString("Char: %1, GridSize: %2").arg(QChar(m_unicode)).arg(m_bitmapDimension);
    }

    // Оператор == для QHash
    bool operator==(const GlyphKey &other) const {
        return m_unicode == other.m_unicode &&
               m_bitmapDimension == other.m_bitmapDimension;
    }

    // Сеттеры
    void setCharacter(int newCharacter) {m_unicode = newCharacter;}
    void setBitmapDimension(int newGridSize) { m_bitmapDimension = newGridSize; }

    // Геттеры
    int character() const { return m_unicode; }
    int bitmapDimension() const { return m_bitmapDimension; }

    QChar getQChar ()
    {
        return QChar(m_unicode);
    }


private:
    char16_t m_unicode;
    int m_bitmapDimension;
};

// inline uint qHash(const GlyphKey &key, uint seed = 0);
inline uint qHash(const GlyphKey &key, uint seed) {
    QtPrivate::QHashCombine hash;
    
    seed = hash(seed, key.character());
    seed = hash(seed, key.bitmapDimension());
    // seed = hash(seed, key.fontFamily());

    return seed;
}


#endif // GLYPHKEY_H
