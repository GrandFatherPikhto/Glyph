#ifndef GLYPHCONTEXT_H
#define GLYPHCONTEXT_H

#include <QChar>
#include <QFont>

#include "bitmapdimension.h"
#include "glyphkey.h"
#include "glyphentry.h"
#include "glyphoffset.h"

class GlyphContext {

public:
    GlyphContext(QSharedPointer<BitmapDimension> bitmapDimension, QSharedPointer<GlyphEntry> glyphEntry) 
        : m_bitmapDimension(bitmapDimension)
        , m_glyphEntry(glyphEntry)
    {}

    const GlyphKey & key() { return m_glyphEntry->glyphKey(); }
    QSharedPointer<GlyphEntry> glyphEntry () const { return m_glyphEntry; }
    QSharedPointer<GlyphEntry> glyphEntry () { return m_glyphEntry; }
    QSharedPointer<BitmapDimension> bitmapDimension() const { return m_bitmapDimension; }
    QSharedPointer<BitmapDimension> bitmapDimension() { return m_bitmapDimension; }

    // Обёртки
    QChar character() {
        if (m_glyphEntry.isNull())
            return QChar ();
        return m_glyphEntry->character();
    }

    int dimension ()
    {
        if (m_glyphEntry.isNull())
            return -1;
        return m_bitmapDimension->bitmapDimension();
    }

    const QFont & glyphFont ()
    {
        Q_ASSERT(m_glyphEntry != nullptr);
        return m_glyphEntry->glyphFont();
    }

    void offsetReset () { m_glyphEntry->glyphOffset().reset (); }
    int offsetLeft() { return m_glyphEntry->glyphOffset().left(); }
    int offsetUp() { return m_glyphEntry->glyphOffset().up(); }
    int offsetRight() { return m_glyphEntry->glyphOffset().right(); }
    int offsetDown() { return m_glyphEntry->glyphOffset().down(); }

#ifndef QT_NO_DATASTREAM
    friend inline QDataStream & operator<<(QDataStream &out, const GlyphContext &context);
    friend inline QDataStream & operator>>(QDataStream &in, GlyphContext &context);
#endif
    

private:
    QSharedPointer<GlyphEntry> m_glyphEntry;
    QSharedPointer<BitmapDimension> m_bitmapDimension;
};

#ifndef QT_NO_DATASTREAM
inline QDataStream & operator<<(QDataStream &out, const GlyphContext &context)
{
    out << *(context.m_glyphEntry.data())
        << *(context.m_bitmapDimension.data());

    return out;
}

inline QDataStream & operator>>(QDataStream &in, GlyphContext &context)
{
    in >> *(context.m_glyphEntry.data())
       >> *(context.m_bitmapDimension.data());

    return in;
}
#endif

#ifndef QT_NO_DEBUG_OUTPUT
inline QDebug operator<<(QDebug debug, const GlyphContext &context)
{
    QDebugStateSaver saver(debug); // Для автоматического сохранения состояния
    debug.nospace() << "GlyphContext("
        << *(context.glyphEntry().data())
        << ", "
        << *(context.bitmapDimension().data())
        << ")";

    return debug;
}
#endif

#endif // GLYPHCONTEXT_H