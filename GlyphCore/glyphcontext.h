#ifndef GLYPHCONTEXT_H
#define GLYPHCONTEXT_H

#include <QChar>
#include <QFont>

#include "bitmapdimension.h"
#include "glyphkey.h"
#include "glyphentry.h"

class GlyphContext {

public:
    GlyphContext(QSharedPointer<BitmapDimension> bitmapDimension, QSharedPointer<GlyphEntry> glyphEntry) 
        : m_bitmapDimension(bitmapDimension)
        , m_glyphEntry(glyphEntry)
    {}

    const GlyphKey & key() { return m_glyphEntry->glyphKey(); }
    QSharedPointer<GlyphEntry> glyphEntry () { return m_glyphEntry; }
    QSharedPointer<BitmapDimension> bitmapDimension() { return m_bitmapDimension; }

private:
    QSharedPointer<GlyphEntry> m_glyphEntry;
    QSharedPointer<BitmapDimension> m_bitmapDimension;
};

#endif // GLYPHCONTEXT_H