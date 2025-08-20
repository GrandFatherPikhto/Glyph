#ifndef FTRENDERER_H
#define FTRENDERER_H

#include <QColor>
#include <QSharedPointer>

#include "iglyphrenderer.h"
#include "imagecontext.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H


class FtRenderer : public IGlyphRenderer {
    public:
        explicit FtRenderer();
        ~FtRenderer() override;

        bool renderGlyph(const FontContext &font, QSharedPointer<ImageContext> image) override;
        virtual QString rendererName() const override { return "FtRenderer"; }

    private:
        void initFtLibrary();
        void releaseFtLibrary();
        void releaseFtFace();
        void loadFtFace();
        void setGlyphSize();
        void loadImage();
        void renderGlyph();
        void saveGlyphSize();

        FT_Library m_ftLibrary;
        FT_Face m_ftFace;
        FT_Error m_ftError;
        FontContext m_font;
        QSharedPointer<ImageContext> m_image;
};

#endif // FTRENDERER_H
