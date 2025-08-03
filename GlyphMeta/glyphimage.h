#ifndef GLYPHIMAGE_H_
#define GLYPHIMAGE_H_

#include <QSharedPointer>
#include <QColor>
#include <QSize>
#include <QImage>

#include "bitmapdimension.h"
#include "glyphcontext.h"
#include "iglyphrenderer.h"
#include "glyphmarkup.h"

#include "GlyphMeta_global.h"

class GLYPHMETA_EXPORT GlyphImage {
public:
    GlyphImage(QSharedPointer<GlyphContext> glyphContext) : m_glyphContext(glyphContext), m_dirty(true) {}
    ~GlyphImage() {};

    void setDirty() { m_dirty; }
    void resetDirty() { m_dirty; }
    bool isDirty() { return m_dirty; }

    void renderImage(QSharedPointer<IGlyphRenderer> renderer, const QColor &color, const QColor &bgColor, const QSize &size = QSize())
    { 
        renderer->renderGlyph(m_glyphContext, m_image, m_glyphMarkup, color, bgColor, size); 
    }

    const GlyphMarkup & glyphMarkup () { return m_glyphMarkup; }

    QSharedPointer<QImage> image() { return m_image; }

private:
    QSharedPointer<GlyphContext> m_glyphContext;
    QSharedPointer<QImage> m_image;
    GlyphMarkup m_glyphMarkup;
    bool m_dirty;
};

#endif // GLYPHIMAGE_H_