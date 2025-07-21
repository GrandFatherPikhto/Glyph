#include "drawglyphrenderer.h"

DrawGlyphRenderer::DrawGlyphRenderer() {}

QSharedPointer<QImage> DrawGlyphRenderer::renderGlyph(
    QSharedPointer<GlyphMeta> glyphMeta, 
    const QColor &glyphColor, 
    const QColor &bgColor,
    const QSize &targetSize 
    )
{
    Q_UNUSED(bgColor)
    m_glyphMeta = glyphMeta;
    QImage image = QImage(targetSize, QImage::Format_ARGB32);
    image.fill(bgColor);
    return QSharedPointer<QImage>::create(image);
}

QString DrawGlyphRenderer::rendererName() const
{
    return "UserGlyphRender";
}
