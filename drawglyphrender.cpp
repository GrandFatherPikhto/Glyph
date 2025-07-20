#include "drawglyphrender.h"

DrawGlyphRender::DrawGlyphRender() {}

QSharedPointer<QImage> DrawGlyphRender::renderGlyph(
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

QString DrawGlyphRender::rendererName() const
{
    return "UserGlyphRender";
}
