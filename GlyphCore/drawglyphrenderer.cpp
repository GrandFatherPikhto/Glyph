#include "drawglyphrenderer.h"

DrawGlyphRenderer::DrawGlyphRenderer()
    : m_targetSize(QSize())
{

}

QSharedPointer<QImage> DrawGlyphRenderer::renderGlyph(
    QSharedPointer<GlyphMeta> glyphMeta, 
    const QColor &glyphColor, 
    const QColor &bgColor,
    const QSize &targetSize 
    )
{
    Q_UNUSED(bgColor)
    m_glyphMeta = glyphMeta;
    m_targetSize = targetSize;

    if (targetSize == QSize())
    {
        m_targetSize = QSize(m_glyphMeta->bitmapDimension(), m_glyphMeta->bitmapDimension());
    }

    QImage image = QImage(m_targetSize, QImage::Format_ARGB32);
    image.fill(bgColor);

    m_renderRect = image.rect();

    return QSharedPointer<QImage>::create(image);
}

QString DrawGlyphRenderer::rendererName() const
{
    return "UserGlyphRender";
}
