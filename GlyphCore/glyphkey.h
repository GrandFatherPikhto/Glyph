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
    GlyphKey(int character = -1, int bitmapDimension = -1)
    : m_unicode(character)
    , m_bitmapDimension(bitmapDimension)
    {

    }

    GlyphKey(const QChar &character, int bitmapDimension)
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
    bool operator == (QSharedPointer<GlyphKey> glyphKey) const {
        return m_unicode == glyphKey->m_unicode
               && m_bitmapDimension == glyphKey->m_bitmapDimension;
    }

    bool operator == (const GlyphKey &glyphKey) const {
        return m_unicode == glyphKey.m_unicode
               && m_bitmapDimension == glyphKey.m_bitmapDimension;
    }

    bool operator != (QSharedPointer<GlyphKey> glyphKey) const {
        return m_unicode != glyphKey->m_unicode 
               || m_bitmapDimension != glyphKey->m_bitmapDimension;
    }

    bool operator != (const GlyphKey &glyphKey) const {
        return m_unicode != glyphKey.m_unicode
               || m_bitmapDimension != glyphKey.m_bitmapDimension;
    }

    const GlyphKey & operator= (const GlyphKey &key)
    {
        m_unicode = key.m_unicode;
        m_bitmapDimension = key.m_bitmapDimension;

        return *this;
    }

    bool isValid ()
    {
        return (m_bitmapDimension > 0 && m_unicode > 0);
    }

    // Сеттеры
    void setUnicode(int newCharacter) {m_unicode = newCharacter;}
    void setCharacter(const QChar &character) { m_unicode = character.unicode(); }
    void setBitmapDimension(int newGridSize) { m_bitmapDimension = newGridSize; }

    // Геттеры
    int unicode() const { return m_unicode; }
    QChar character () const { return QChar(m_unicode); }
    int bitmapDimension() const { return m_bitmapDimension; }

    QChar getQChar ()
    {
        return QChar(m_unicode);
    }

#ifndef QT_NO_DATASTREAM
    friend inline QDataStream & operator<<(QDataStream &out, const GlyphKey &key);
    friend inline QDataStream & operator>>(QDataStream &in, GlyphKey &key);
#endif


private:
    quint32 m_unicode;
    int m_bitmapDimension;
};

// inline uint qHash(const GlyphKey &key, uint seed = 0);
inline uint qHash(const GlyphKey &key, uint seed) {
    QtPrivate::QHashCombine hash;
    
    seed = hash(seed, key.unicode());
    seed = hash(seed, key.bitmapDimension());
    // seed = hash(seed, key.fontFamily());

    return seed;
}

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator<<(QDataStream &out, const GlyphKey &key)
{
    out << key.m_unicode
        << key.m_bitmapDimension;

    return out;
}

inline QDataStream & operator>>(QDataStream &in, GlyphKey &key)
{
    in >> key.m_unicode
       >> key.m_bitmapDimension;
       
    return in;
}
#endif


#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator<<(QDebug debug, const GlyphKey &key)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() 
        << "GlyphKey(character: "
        << key.character()
        << ", dimension: "
        << key.bitmapDimension()
        << ")";

    return debug;
}

inline QDebug operator<<(QDebug debug, const GlyphKey* pKey) {
    if (!pKey) return debug << "GlyphContext(nullptr)";
    return debug << *pKey;
}

#endif


Q_DECLARE_METATYPE(GlyphKey)

#endif // GLYPHKEY_H
