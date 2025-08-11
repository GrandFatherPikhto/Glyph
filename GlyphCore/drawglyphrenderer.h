#ifndef USERGLYPHRENDER_H
#define USERGLYPHRENDER_H

#include <QSharedPointer>
#include <QRect>
#include <QImage>

#include "glyphcontext.h"
#include "glyphmarkup.h"
#include "iglyphrenderer.h"

#include "GlyphCore_global.h"

class AppContext;

class GLYPHCORE_EXPORT DrawGlyphRenderer : public IGlyphRenderer
{
public:
    DrawGlyphRenderer();
    bool renderGlyph(
            QSharedPointer<GlyphContext> context,
            QSharedPointer<QImage> image,
            GlyphMarkup &glyphMarkup,
            const QColor &color,
            const QColor &bgColor,
            const QSize &size
        ) override;

    // В freetyperender.h добавьте объявление:
    QString rendererName() const override;

private:

};

#endif // USERGLYPHRENDER_H
