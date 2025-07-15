#ifndef __FREETYPERENDER_H__
#define __FREETYPERENDER_H__


#include <QSharedPointer>
#include <QFontMetrics>
#include <QPainter>
#include <QImage>

#include "glyphmeta.h"

#include "iglyphrender.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H

class FreeTypeRender : public IGlyphRender {
public:
    explicit FreeTypeRender();
    ~FreeTypeRender();

    QSharedPointer<QImage> renderGlyph(
        QSharedPointer<GlyphMeta> glyphMeta,
        const QSize& targetSize,
        Qt::TransformationMode mode
    ) override;

    // В freetyperender.h добавьте объявление:
    QString rendererName() const override;

private:
    void loadFontFace ();
    void loadGlyph ();
    void doneFace ();
    void doneLibrary ();
    void setTargetSize (const QSize &targetSize);

    QSharedPointer<GlyphMeta> m_glyphMeta;
    FT_Error m_ftError;
    FT_Library m_ftLibrary;
    FT_Face m_ftFace;    
};

#endif // __FREETYPERENDER_H__

