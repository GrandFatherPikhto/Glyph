#ifndef __IGLYPHRENDER_H__
#define __IGLYPHRENDER_H__

#include <QSharedPointer>

#include "glyphcontext.h"
#include "glyphmarkup.h"

#include <QImage>

class IGlyphRenderer {
public:
    // IGlyphRenderer () = default;
    // virtual ~IGlyphRenderer() = default;
    
    // Основной метод рендеринга
    virtual bool renderGlyph(QSharedPointer<GlyphContext> context, QSharedPointer<QImage> image, GlyphMarkup &glyphMarkup, const QColor &color, const QColor &bgColor, const QSize &targetSize) = 0;

    // Дополнительные возможности
    // virtual bool supportsFeature(GlyphFeature feature) const = 0;
    virtual QString rendererName() const = 0;
};

#endif // __IGLYPHRENDER_H__
