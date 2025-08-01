#ifndef GLYPHCONTEXT_H
#define GLYPHCONTEXT_H

#include <QChar>
#include <QFont>

#include "bitmapdimension.h"
#include "glyphkey.h"
#include "glyphoffset.h"

class GlyphContext {

public:
    GlyphContext(QSharedPointer<BitmapDimension> bitmapDimension, const QChar &character, int glyphSize, bool temporary = true, const QFont &font = QFont(), const QString &fontPath = QString()) 
        : m_bitmapDimension(bitmapDimension)
        , m_character(character)
        , m_glyphSize(glyphSize)
        , m_glyphFont(font)
        , m_fontPath(fontPath)
        , m_temporary(temporary)
        , m_glyphOffset{0,0}   
    {
        m_key = GlyphKey(character, bitmapDimension->bitmapDimension());
    }

    const GlyphKey & key() { return m_key; }

    QSharedPointer<BitmapDimension> bitmapDimension() const { return m_bitmapDimension; }

    bool temporary () const { return m_temporary; }
    bool setTemporary () { m_temporary = true; return m_temporary; }
    bool resetTemporary () { m_temporary = false; return m_temporary; }
    void setTemporaryValue (bool value) { m_temporary = value; }

    const QString &fontPath () const { return m_fontPath; }
    const QFont & glyphFont() const { return m_glyphFont; }
    const QChar & character() const { return m_character; }
    int glyphSize () const { return m_glyphSize; }
    const GlyphOffset & glyphOffset() const { return m_glyphOffset; }
    GlyphOffset & glyphOffset() { return m_glyphOffset; }

    GlyphKey & glyphKey() { return m_key; }

    void setGlyphOffset(const GlyphOffset &offset)
    {
        m_glyphOffset = offset;
    }

    void resetGlyphOffset ()
    {
        m_glyphOffset = GlyphOffset{0, 0};
    }

    // Обёртки
    int dimension () const
    {
        return m_bitmapDimension->bitmapDimension();
    }

    void offsetReset () { m_glyphOffset.reset (); }
    int offsetLeft() { return m_glyphOffset.left(); }
    int offsetUp() { return m_glyphOffset.up(); }
    int offsetRight() { return m_glyphOffset.right(); }
    int offsetDown() { return m_glyphOffset.down(); }

#ifndef QT_NO_DATASTREAM
    friend inline QDataStream & operator<<(QDataStream &out, const GlyphContext &context);
    friend inline QDataStream & operator>>(QDataStream &in, GlyphContext &context);
#endif

private:
    QSharedPointer<BitmapDimension> m_bitmapDimension;
    int m_glyphSize;
    bool m_temporary;

    QChar m_character;
    QFont m_glyphFont;
    QString m_fontPath;

    GlyphKey m_key;

    GlyphOffset m_glyphOffset;
};

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator<<(QDataStream &out, const GlyphContext &context)
{
    out << context.m_character
        << context.m_glyphSize
        << context.m_glyphFont
        << context.m_fontPath
        << context.m_temporary
        << *(context.m_bitmapDimension.data());

    return out;
}

inline QDataStream & operator>>(QDataStream &in, GlyphContext &context)
{
    in >> context.m_character
       >> context.m_glyphSize
       >> context.m_glyphFont
       >> context.m_fontPath
       >> context.m_temporary
       >> *(context.m_bitmapDimension.data());

    return in;
}
#endif

#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator<<(QDebug debug, const GlyphContext &context)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "GlyphContext(character: "
        << context.character()
        << ", size: "
        << context.glyphSize()
        << ", font path: "
        << context.fontPath()
        << ", font: "
        << context.glyphFont().family()
        << ", temporary: " << context.temporary()
        << ", "
        << *(context.bitmapDimension().data())
        << ")";

    return debug;
}

inline QDebug operator<<(QDebug debug, const GlyphContext* context) {
    if (!context) return debug << "GlyphContext(nullptr)";
    return debug << *context;
}

#endif

Q_DECLARE_METATYPE(GlyphContext)

#endif // GLYPHCONTEXT_H
