#include "glyphimage.h"

GlyphImage::GlyphImage(QSharedPointer<GlyphContext> glyphContext, QSharedPointer<IGlyphRenderer> renderer)
    : m_glyphContext(glyphContext)
    , m_glyphRenderer(renderer)
{

}

GlyphImage::~GlyphImage()
{
}

void GlyphImage::renderImage(const QColor &color, const QColor &bgColor, const QSize &size)
{
    if(m_glyphRenderer->renderGlyph(m_glyphContext, m_image, m_glyphMarkup, color, bgColor, size));
    {
        m_dirty = false;
    }    
}