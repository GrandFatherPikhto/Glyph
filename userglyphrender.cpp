#include "userglyphrender.h"

UserGlyphRender::UserGlyphRender()
{

}

UserGlyphRender::~UserGlyphRender()
{

}

QSharedPointer<QImage> UserGlyphRender::renderGlyph (
    QSharedPointer<GlyphMeta> glyphMeta,
    const QSize& targetSize,
    Qt::TransformationMode mode
) {
    // Create a QImage from the FreeType bitmap
    QImage image;
    image.fill(Qt::transparent);
    if (targetSize != QSize())
    {
        // QImage::Format_Mono
        image = QImage(targetSize.width(), targetSize.height(), QImage::Format_ARGB32);
    } else
    {
        image = QImage(glyphMeta->gridSize(), glyphMeta->gridSize(), QImage::Format_ARGB32);
    }
    
    return QSharedPointer<QImage>::create(image); 
}
