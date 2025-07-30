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

class GlyphImage {
public:
    GlyphImage(QSharedPointer<GlyphContext> glyphContext, QSharedPointer<IGlyphRenderer> renderer);
    ~GlyphImage();

    bool isDirty() { return m_dirty; }
    void setDirty() { m_dirty = true; }
    void resetDirty() { m_dirty = false; }
    const GlyphMarkup & glyphMarkup () { return m_glyphMarkup; }

    void renderImage (const QColor &color, const QColor &bgColor, const QSize &size = QSize());

    QSharedPointer<QImage> image() { return m_image; }

private:
    QSharedPointer<GlyphContext> m_glyphContext;
    QSharedPointer<IGlyphRenderer> m_glyphRenderer;
    QSharedPointer<QImage> m_image;

    bool m_dirty;
    GlyphMarkup m_glyphMarkup;
};

#endif // GLYPHIMAGE_H_