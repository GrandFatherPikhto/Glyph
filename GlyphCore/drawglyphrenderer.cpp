#include "drawglyphrenderer.h"

DrawGlyphRenderer::DrawGlyphRenderer()
{}

bool DrawGlyphRenderer::renderGlyph(
        QSharedPointer<GlyphContext> context,
        QSharedPointer<QImage> image,
        GlyphMarkup &glyphMarkup,
        const QColor &color,
        const QColor &bgColor,
        const QSize &size
    )
{
    Q_UNUSED(bgColor)

    QSize targetSize = size;

    if (targetSize == QSize())
    {
        targetSize = QSize(context->dimension(), context->dimension());
    }

    image = QSharedPointer<QImage>::create(targetSize, QImage::Format_ARGB32);
    image->fill(bgColor);

    glyphMarkup.setLeft(0);
    glyphMarkup.setTop(0);
    // glyphMarkup.setOffsetX(0);
    // glyphMarkup.setOffsetY(0);
    glyphMarkup.setWidth(targetSize.width());
    glyphMarkup.setHeight(targetSize.height());

    return true;
}

QString DrawGlyphRenderer::rendererName() const
{
    return "UserGlyphRender";
}
