#ifndef __IGLYPHRENDER_H__
#define __IGLYPHRENDER_H__

#include <QSharedPointer>

#include "glyphmeta.h"

class IGlyphRender {
public:
    // IGlyphRender () = default;
    // virtual ~IGlyphRender() = default;
    
    // Основной метод рендеринга
    virtual QSharedPointer<QImage> renderGlyph(QSharedPointer<GlyphMeta> glyphMeta, const QSize& targetSize = QSize(), Qt::TransformationMode mode = Qt::FastTransformation) = 0;

    // Дополнительные возможности
    // virtual bool supportsFeature(GlyphFeature feature) const = 0;
    virtual QString rendererName() const = 0;
};

#endif // __IGLYPHRENDER_H__
