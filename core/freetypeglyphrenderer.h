#ifndef __FREETYPERENDER_H__
#define __FREETYPERENDER_H__


#include <QSharedPointer>
#include <QFontMetrics>
#include <QPainter>
#include <QImage>

#include "glyphmeta.h"

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
    void calcRenderRect ();
    void loadFontFace ();
    void loadGlyph ();
    void doneFace ();
    void doneLibrary ();
    void setTargetSize (const QSize &targetSize);

    QSharedPointer<GlyphMeta> m_glyphMeta;
    QRect m_renderRect;
    QSize m_targetSize;
    FT_Error m_ftError;
    FT_Library m_ftLibrary;
    FT_Face m_ftFace;
    FT_GlyphSlot m_slot;
    FT_Bitmap m_bitmap;
};

#endif // __FREETYPERENDER_H__

