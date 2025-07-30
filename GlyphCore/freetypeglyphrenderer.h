#ifndef __FREETYPERENDER_H__
#define __FREETYPERENDER_H__


#include <QSharedPointer>
#include <QFontMetrics>
#include <QPainter>
#include <QImage>

#include "glyphcontext.h"
#include "iglyphrenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H

class FreeTypeGlyphRenderer : public IGlyphRenderer {
public:
    explicit FreeTypeGlyphRenderer();
    ~FreeTypeGlyphRenderer();

    bool renderGlyph(
        QSharedPointer<GlyphContext> context,
        QSharedPointer<QImage> image,
        GlyphMarkup &glyphMarkup,
        const QColor &color,
        const QColor &bgColor,
        const QSize &size
    ) override;

    QString rendererName() const override;

private:
    void createImage (const QColor &color, const QColor &bgColor);
    void setGlyphMarkup (GlyphMarkup &markup);
    void loadFontFace ();
    void loadGlyph ();
    void doneFace ();
    void doneLibrary ();
    void setTargetSize (const QSize &targetSize);

    QSharedPointer<GlyphContext> m_glyph;
    QSharedPointer<QImage> m_image;
    QSize m_targetSize;
    FT_Error m_ftError;
    FT_Library m_ftLibrary;
    FT_Face m_ftFace;
    FT_GlyphSlot m_slot;
    FT_Bitmap m_bitmap;
};

#endif // __FREETYPERENDER_H__

