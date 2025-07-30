#ifndef GLYPHMETA_H_
#define GLYPHMETA_H_

#include <QChar>
#include <QFont>

#include "bitmapdimension.h"
#include "glyphkey.h"

class GlyphMeta {
    public:
    GlyphMeta(const QChar &character, int bitmapDimension, int glyphSize = -1, const QFont &font = QFont(), bool temporary = true)
        : m_character(character)
        , m_font(font)
        , m_glyphSize(glyphSize)
        {
            
        }

    private:
    
    int m_glyphSize;
    
    QChar m_character;
    QFont m_font;

    BitmapDimension m_bitmapDimension;
    // GlyphRender m_templateLayer;
    // GlyphRender m_previewLayer;
    // GlyphRender m_drawLayer;
};

#endif // GLYPHMETA_H_