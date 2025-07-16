#ifndef USERGLYPHRENDER_H
#define USERGLYPHRENDER_H

#include <QSharedPointer>
#include <QRect>
#include <QImage>

#include "glyphmeta.h"
#include "iglyphrender.h"

class UserGlyphRender : public IGlyphRender
{
public:
    UserGlyphRender();
    QSharedPointer<QImage> renderGlyph(
        QSharedPointer<GlyphMeta> glyphMeta,
        const QSize &targetSize,
        const QColor &glyphColor
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
