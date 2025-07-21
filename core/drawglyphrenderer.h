#ifndef USERGLYPHRENDER_H
#define USERGLYPHRENDER_H

#include <QSharedPointer>
#include <QRect>
#include <QImage>

#include "glyphmeta.h"
#include "iglyphrenderer.h"

class DrawGlyphRenderer : public IGlyphRenderer
{
public:
    DrawGlyphRenderer();
    QSharedPointer<QImage> renderGlyph(
        QSharedPointer<GlyphMeta> glyphMeta,
        const QColor &glyphColor,
        const QColor &bgColor,
        const QSize &targetSize
        ) override;

    // В freetyperender.h добавьте объявление:
    QString rendererName() const override;
    QRect renderRect() const override {
        return m_renderRect;
    }

private:
    QRect m_renderRect;
    QSharedPointer<GlyphMeta> m_glyphMeta;
};

#endif // USERGLYPHRENDER_H
