#ifndef __USERGLYPHRENDER_H__
#define __USERGLYPHRENDER_H__

#include <QSharedPointer>
#include <QImage>

#include "iglyphrender.h"

class UserGlyphRender : public IGlyphRender {
public:
    UserGlyphRender();
    ~UserGlyphRender ();
    QSharedPointer<QImage> renderGlyph (
        QSharedPointer<GlyphMeta> glyphMeta,
        const QSize& targetSize,
        Qt::TransformationMode mode) override;

};

#endif // __USERGLYPHRENDER_H__
