#ifndef __GLYPHENTRY_H__
#define __GLYPHENTRY_H__

#include <QSharedPointer>
#include <QChar>
#include <QFont>

#include "glyphkey.h"
#include "bitmapdimension.h"
#include "glyphoffset.h"

class GlyphEntry {
public:
    GlyphEntry(QSharedPointer<BitmapDimension> bitmapDimension, const QChar &character, int glyphSize, bool temporary = true, const QFont &font = QFont(), const QString &fontPath = QString())
        : m_bitmapDimension(bitmapDimension)
        , m_character(character)
        , m_glyphSize(glyphSize)
        , m_glyphFont(font)
        , m_glyphOffset{0,0}
    {
        m_key = GlyphKey(m_character, m_bitmapDimension->bitmapDimension());
    }

    bool temporary () const { return m_temporary; }
    const QString &fontPath () const { return m_fontPath; }
    const QFont & glyphFont() const { return m_glyphFont; }
    const QChar & character() const { return m_character; }
    int glyphSize () const { return m_glyphSize; }
    const GlyphOffset & glyphOffset() const { return m_glyphOffset; }
    GlyphOffset & glyphOffset() { return m_glyphOffset; }

    GlyphKey & glyphKey() { return m_key; }

    QSharedPointer<BitmapDimension> bitmapDimension () { return m_bitmapDimension; }

    void setTemporary (bool value) { m_temporary = value; }

    void setGlyphOffset(const GlyphOffset &offset)
    {
        m_glyphOffset = offset;
    }

    void resetGlyphOffset ()
    {
        m_glyphOffset = GlyphOffset{0, 0};
    }

    void offsetReset () { m_glyphOffset.reset(); }
    int offsetLeft() { return m_glyphOffset.left(); }
    int offsetUp() { return m_glyphOffset.up(); }
    int offsetRight() { return m_glyphOffset.right (); }
    int offsetDown() { return m_glyphOffset.down(); }

#ifndef QT_NO_DATASTREAM
    friend inline QDataStream & operator<<(QDataStream &out, const GlyphEntry &entry);
    friend inline QDataStream & operator>>(QDataStream &in, GlyphEntry &entry);
#endif


private:
    int m_glyphSize;
    bool m_temporary;

    QChar m_character;
    QFont m_glyphFont;
    QString m_fontPath;

    GlyphKey m_key;

    GlyphOffset m_glyphOffset;

    QSharedPointer<BitmapDimension> m_bitmapDimension;
};

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator<<(QDataStream &out, const GlyphEntry &entry)
{
    out << entry.m_character
        << entry.m_glyphSize
        << entry.m_glyphFont
        << entry.m_fontPath
        << entry.m_temporary
        << *(entry.m_bitmapDimension.data());

    return out;
}

inline QDataStream & operator>>(QDataStream &in, GlyphEntry &entry)
{
    in >> entry.m_character
       >> entry.m_glyphSize
       >> entry.m_glyphFont
       >> entry.m_fontPath
       >> entry.m_temporary
       >> *(entry.m_bitmapDimension.data());

    return in;
}
#endif

#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator<<(QDebug debug, const GlyphEntry &entry)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "GlyphEntry("
        << entry.character()
        << ", "
        << entry.glyphFont()
        << ", "
        << entry.glyphSize()
        << ", "
        << entry.glyphOffset()
        << ")";
    return debug;
}
#endif

Q_DECLARE_METATYPE(GlyphEntry)

#endif // __GLYPHENTRY_H__