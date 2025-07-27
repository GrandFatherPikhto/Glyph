#ifndef __IGLYPHRENDER_H__
#define __IGLYPHRENDER_H__

#include <QSharedPointer>

#include "glyphmeta.h"

class IGlyphRenderer {
public:
    // IGlyphRenderer () = default;
    // virtual ~IGlyphRenderer() = default;
    
    // Основной метод рендеринга
    virtual QSharedPointer<QImage> renderGlyph(QSharedPointer<GlyphMeta> glyphMeta, const QColor &glyphColor, const QColor &bgColor, const QSize &targetSize) = 0;
    virtual QRect renderRect () const = 0;

    // Дополнительные возможности
    // virtual bool supportsFeature(GlyphFeature feature) const = 0;
    virtual QString rendererName() const = 0;
};

#endif // __IGLYPHRENDER_H__
