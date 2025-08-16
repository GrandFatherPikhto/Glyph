#ifndef FTRENDERER_H
#define FTRENDERER_H

#include "iglyphrenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H


class FtRenderer : public IGlyphRenderer {
    public:
        FtRenderer();
        ~FtRenderer() override;

        bool renderGlyph(const QColor &color, const QColor &bgColor, const QSize &targetSize) override;

private:
    FT_Library m_ftLibrary;
    FT_Face m_ftFace;
};

#endif // FTRENDERER_H