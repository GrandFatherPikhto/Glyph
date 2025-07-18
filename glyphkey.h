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
    GlyphKey(int character /* = -1 */, int bitmapDimension /* = -1 */, const QString &fontFamily)
    : m_unicode(character)
    , m_bitmapDimension(bitmapDimension)
    , m_fontFamily(fontFamily)
    {

    }

    GlyphKey(const QChar &character, int bitmapDimension, const QFont &font)
        : m_unicode(character.unicode())
        , m_bitmapDimension(bitmapDimension)
        , m_fontFamily(font.family())
    {
    
    }

    GlyphKey(const GlyphKey &key)
        : m_unicode(key.m_unicode)
        , m_bitmapDimension(key.m_bitmapDimension)
        , m_fontFamily(key.m_fontFamily)
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
               m_bitmapDimension == other.m_bitmapDimension &&
               m_fontFamily == other.m_fontFamily;
    }

    // Сеттеры
    void setCharacter(int newCharacter) {m_unicode = newCharacter;}
    void setBitmapDimension(int newGridSize) { m_bitmapDimension = newGridSize; }
    void setFontFamily(const QString fontFamily) { m_fontFamily = fontFamily; }

    // Геттеры
    int character() const { return m_unicode; }
    int bitmapDimension() const { return m_bitmapDimension; }
    const QString & fontFamily() const { return m_fontFamily; }

    QChar getQChar ()
    {
        return QChar(m_unicode);
    }


private:
    char16_t m_unicode;
    int m_bitmapDimension;
    QString m_fontFamily;
};

// inline uint qHash(const GlyphKey &key, uint seed = 0);
inline uint qHash(const GlyphKey &key, uint seed) {
    QtPrivate::QHashCombine hash;
    
    seed = hash(seed, key.character());
    seed = hash(seed, key.bitmapDimension());
    seed = hash(seed, key.fontFamily());

    return seed;
}


#endif // GLYPHKEY_H
