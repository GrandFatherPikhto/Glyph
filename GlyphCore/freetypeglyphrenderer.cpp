#include "freetypeglyphrenderer.h"

FreeTypeGlyphRenderer::FreeTypeGlyphRenderer()
    : m_ftError(FT_Err_Ok)
    , m_ftLibrary(0)
    , m_ftFace(0)
    , m_slot(0)
    , m_image(QSharedPointer<QImage>())
{
    if (FT_Init_FreeType(&m_ftLibrary)) {
        throw std::runtime_error("Failed to initialize FreeType");
    }
}

FreeTypeGlyphRenderer::~FreeTypeGlyphRenderer()
{
    doneFace ();
    doneLibrary ();
}

QString FreeTypeGlyphRenderer::rendererName() const
{
    return "FreeTypeGlyphRenderer";
}

bool FreeTypeGlyphRenderer::renderGlyph (
    QSharedPointer<GlyphContext> glyph,
    QSharedPointer<QImage> image,
    GlyphMarkup &glyphMarkup,
    const QColor &color,
    const QColor &bgColor,
    const QSize &targetSize
    ) 
{
    m_glyph = glyph;

    if (m_glyph.isNull() || m_image.isNull()) 
    {
        return false;
    }

    loadFontFace();
    loadGlyph();
    setGlyphMarkup(glyphMarkup);
    createImage(color, bgColor);

    image = m_image;
    return true;
}

void FreeTypeGlyphRenderer::createImage(const QColor &color, const QColor &bgColor)
{
    m_image = QSharedPointer<QImage>::create(QSize(m_bitmap.width, m_bitmap.rows), QImage::Format_ARGB32);
    m_image->fill(bgColor);

    QColor pixelColor(color);

    for (int y = 0; y < m_bitmap.rows; ++y) {
        for (int x = 0; x < m_bitmap.width; ++x) {
            uint8_t pixel;
            if (m_bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {
                pixel = (m_bitmap.buffer[y * m_bitmap.pitch + (x / 8)] >> (7 - (x % 8))) & 1 ? 0xEF : 0;
            } else { // FT_PIXEL_MODE_GRAY
                pixel = m_bitmap.buffer[y * m_bitmap.pitch + x];
            }

            pixelColor.setAlpha(pixel ? color.alpha() : 0);
            m_image->setPixelColor(x, y, pixelColor);
        }
    }
}

void FreeTypeGlyphRenderer::loadFontFace()
{
    doneFace();
    m_ftError = FT_New_Face(m_ftLibrary, m_glyph->glyphEntry()->fontPath().toStdString().c_str(), 0, &m_ftFace);
    if (m_ftError)
    {
        throw std::runtime_error("Failed to load font: " + m_glyph->glyphEntry()->fontPath().toStdString());
    }
}

void FreeTypeGlyphRenderer::loadGlyph ()
{
    FT_UInt glyphIndex = FT_Get_Char_Index(m_ftFace, m_glyph->glyphEntry()->character().unicode());
    m_ftError = FT_Load_Glyph(m_ftFace, glyphIndex, FT_LOAD_RENDER | FT_LOAD_TARGET_MONO);

    if (m_ftError)
    {
        throw std::runtime_error("Failed to load glyph: " + QString(m_glyph->glyphEntry()->character()).toStdString());
    }

    // Get the glyph slot
    m_slot = m_ftFace->glyph;
    m_bitmap = m_slot->bitmap;
}

void FreeTypeGlyphRenderer::doneFace()
{
    if (m_ftFace)
    {
        FT_Done_Face(m_ftFace);
        m_ftFace = 0;
    }
}

void FreeTypeGlyphRenderer::doneLibrary()
{
    if (m_ftLibrary)
    {
        FT_Done_FreeType(m_ftLibrary);
    }
}

void FreeTypeGlyphRenderer::setTargetSize(const QSize &targetSize)
{
    int sizeX = m_glyph->glyphEntry()->glyphSize();
    int sizeY = m_glyph->glyphEntry()->glyphSize();
    
    if (targetSize != QSize())
    {
        sizeX = targetSize.width();
        sizeY = targetSize.height();
    }

    m_ftError = FT_Set_Pixel_Sizes(m_ftFace, sizeX, sizeY);

    if (m_ftError)
    {
        throw std::runtime_error(QString("Can't set %1 to size %2").arg(
            m_glyph->glyphEntry()->character(), 
            QString::number(m_glyph->glyphEntry()->glyphSize())
        ).toStdString());
    }
}

void FreeTypeGlyphRenderer::setGlyphMarkup (GlyphMarkup &markup)
{
    markup.setWidth(m_bitmap.width);
    markup.setHeight(m_bitmap.rows);
    markup.setLeft(m_slot->bitmap_left);
    markup.setTop(m_slot->bitmap_top);
}
