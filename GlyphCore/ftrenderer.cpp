#include "ftrenderer.h"

FtRenderer::FtRenderer()
    : IGlyphRenderer ()
    , m_ftLibrary(0)
    , m_ftFace(0)
    , m_ftError(FT_Err_Ok)
{

}

FtRenderer::~FtRenderer()
{
    releaseFtFace();
    releaseFtLibrary();
}

bool FtRenderer::renderGlyph(const FontContext &font, QSharedPointer<ImageContext> image)
{
    if (!(font.isValid() && image->isValid()))
        return false;

    m_ftError = FT_Err_Ok;

    m_font = font;
    m_image = image;

    loadFtFace();
    setGlyphSize();
    renderGlyph();
    saveGlyphSize();
    loadImage();

    m_image->setDirty(false);
    // image = m_image;

    return true;
}

void FtRenderer::releaseFtLibrary()
{
    if (m_ftLibrary != 0)
    {
        FT_Done_Library(m_ftLibrary);
        m_ftLibrary = 0;
    }
}

void FtRenderer::releaseFtFace()
{
    if(m_ftFace != 0)
    {
        FT_Done_Face(m_ftFace);
        m_ftFace = 0;
    }
}

void FtRenderer::initFtLibrary()
{
    releaseFtLibrary();
    m_ftError = FT_Init_FreeType(&m_ftLibrary);
    if (m_ftError)
    {
        qWarning() << __FILE__ << __LINE__ << "Error loading FTLibrary";
    }
}

void FtRenderer::loadFtFace()
{
    initFtLibrary();
    if (m_ftError)
        return;

    m_ftError = FT_New_Face(m_ftLibrary, m_font.fileName().toStdString().c_str(), 0, &m_ftFace);
    if (m_ftError)
    {
        qWarning() << __FILE__ << __LINE__ << "Error face loading" << m_font.fileName();
    }
}

void FtRenderer::setGlyphSize()
{
    if (m_image->size() == QSize() || m_ftError)
        return;

    int w = m_image->size().width();
    int h = m_image->size().height();

    m_ftError = FT_Set_Pixel_Sizes(m_ftFace, w, h);

    if (m_ftError)
    {
        throw std::runtime_error(QString("Can't set %1 to size (%2x%3").arg(
           m_image->character()
           , QString::number(m_image->size().width())
           , QString::number(m_image->size().height())
           ).toStdString());
    }
}

void FtRenderer::renderGlyph ()
{
    if (m_ftError)
        return;

    FT_UInt glyphIndex = FT_Get_Char_Index(m_ftFace, m_image->character().unicode());
    m_ftError = FT_Load_Glyph(m_ftFace, glyphIndex, FT_LOAD_RENDER | FT_LOAD_TARGET_MONO);

    if (m_ftError)
    {
        throw std::runtime_error("Failed to load glyph: " + QString(m_image->character()).toStdString());
    }
}

void FtRenderer::loadImage()
{
    if (m_ftError)
        return;
    QColor pixelColor(m_image->color());
    FT_Bitmap bitmap = m_ftFace->glyph->bitmap;
#ifdef IMAGECONTEXT_SHARED_POINTER    
    QSharedPointer<QImage> image = QSharedPointer<QImage>::create(QSize(bitmap.width, bitmap.rows), QImage::Format_ARGB32);
#else
    QImage image(QSize(bitmap.width, bitmap.rows), QImage::Format_ARGB32);
#endif    

    for (int y = 0; y < bitmap.rows; ++y) {
        for (int x = 0; x < bitmap.width; ++x) {
            uint8_t pixel;
            if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {
                pixel = (bitmap.buffer[y * bitmap.pitch + (x / 8)] >> (7 - (x % 8))) & 1 ? 0xEF : 0;
            } else { // FT_PIXEL_MODE_GRAY
                pixel = bitmap.buffer[y * bitmap.pitch + x];
            }

            pixelColor.setAlpha(pixel ? m_image->color().alpha() : 0);
#ifdef IMAGECONTEXT_SHARED_POINTER                
            image->setPixelColor(x, y, pixelColor);
#else
            image.setPixelColor(x, y, pixelColor);
#endif            
        }
    }

    m_image->setImage(image);
}

void FtRenderer::saveGlyphSize()
{
    if (m_ftError)
        return;

    m_image->setLeft(m_ftFace->glyph->bitmap_left);
    m_image->setTop(m_ftFace->glyph->bitmap_top);
    m_image->setWidth(m_ftFace->glyph->bitmap.width);
    m_image->setHeight(m_ftFace->glyph->bitmap.rows);
}
