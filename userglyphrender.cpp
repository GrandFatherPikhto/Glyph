#include "userglyphrender.h"

UserGlyphRender::UserGlyphRender() {}

QSharedPointer<QImage> UserGlyphRender::renderGlyph(QSharedPointer<GlyphMeta> glyphMeta, const QSize &targetSize, const QColor &glyphColor)
{
    m_glyphMeta = glyphMeta;
    QImage image = QImage(targetSize, QImage::Format_ARGB32);
    image.fill(Qt::white);
    return QSharedPointer<QImage>::create(image);
}

QString UserGlyphRender::rendererName() const
{
    return "UserGlyphRender";
}
