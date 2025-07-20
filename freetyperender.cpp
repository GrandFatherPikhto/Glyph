#include "freetyperender.h"

FreeTypeRender::FreeTypeRender()
    : m_ftError(FT_Err_Ok)
    , m_ftLibrary(0)
    , m_ftFace(0)
    , m_slot(0)
    , m_renderRect(QRect())
{
    if (FT_Init_FreeType(&m_ftLibrary)) {
        throw std::runtime_error("Failed to initialize FreeType");
    }
}

FreeTypeRender::~FreeTypeRender()
{
    doneFace ();
    doneLibrary ();
}

// В freetyperender.cpp добавьте реализацию:
QString FreeTypeRender::rendererName() const
{
    return "FreeTypeRender";
}

QSharedPointer<QImage> FreeTypeRender::renderGlyph (
    QSharedPointer<GlyphMeta> glyphMeta,
    const QColor &glyphColor,
    const QColor &bgColor,
    const QSize &targetSize
    ) 
{
    m_glyphMeta = glyphMeta;
    m_targetSize = targetSize;
    
    if (m_glyphMeta.isNull() || !m_glyphMeta->isValid() || m_glyphMeta->fontPath().isEmpty()) {
        return QSharedPointer<QImage>();
    }

    loadFontFace();

    setTargetSize();
    loadGlyph();

    calcRenderRect();

    QImage image = QImage(QSize(m_bitmap.width, m_bitmap.rows), QImage::Format_ARGB32);
    image.fill(bgColor);

    QColor pixelColor(glyphColor);

    for (int y = 0; y < m_bitmap.rows; ++y) {
        for (int x = 0; x < m_bitmap.width; ++x) {
            uint8_t pixel;
            // Обрабатываем монохромный (1 бит на пиксель) и grayscale (8 бит)
            if (m_bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {
                pixel = (m_bitmap.buffer[y * m_bitmap.pitch + (x / 8)] >> (7 - (x % 8))) & 1 ? 0xEF : 0;
            } else { // FT_PIXEL_MODE_GRAY
                pixel = m_bitmap.buffer[y * m_bitmap.pitch + x];
            }

            // Устанавливаем пиксель в QImage (чёрный с альфа-каналом)
            // image.setPixel(x, y, qRgba(0xFF, 0x33, 0x33, pixel));
            pixelColor.setAlpha(pixel ? glyphColor.alpha() : 0);
            image.setPixelColor(x, y, pixelColor);
        }
    }

    return QSharedPointer<QImage>::create(image);    
}

void FreeTypeRender::loadFontFace()
{
    doneFace();
    m_ftError = FT_New_Face(m_ftLibrary, m_glyphMeta->fontPath().toStdString().c_str(), 0, &m_ftFace);
    if (m_ftError)
    {
        throw std::runtime_error("Failed to load font: " + m_glyphMeta->fontPath().toStdString());
    }
}

void FreeTypeRender::loadGlyph ()
{
    FT_UInt glyphIndex = FT_Get_Char_Index(m_ftFace, m_glyphMeta->character().unicode());
    m_ftError = FT_Load_Glyph(m_ftFace, glyphIndex, FT_LOAD_RENDER | FT_LOAD_TARGET_MONO);

    if (m_ftError)
    {
        throw std::runtime_error("Failed to load glyph: " + QString(m_glyphMeta->character()).toStdString());
    }

    // Get the glyph slot
    m_slot = m_ftFace->glyph;
    m_bitmap = m_slot->bitmap;
}

void FreeTypeRender::doneFace()
{
    if (m_ftFace)
    {
        FT_Done_Face(m_ftFace);
        m_ftFace = 0;
    }
}

void FreeTypeRender::doneLibrary()
{
    if (m_ftLibrary)
    {
        FT_Done_FreeType(m_ftLibrary);
    }
}

void FreeTypeRender::setTargetSize()
{
    m_ftError = FT_Set_Pixel_Sizes(m_ftFace, m_targetSize.width(), m_targetSize.height());

    if (m_ftError)
    {
        throw std::runtime_error(QString("Can't set %1 to size %2").arg(m_glyphMeta->character()).arg(m_glyphMeta->glyphSize()).toStdString());
    }
}

void FreeTypeRender::calcRenderRect ()
{
    m_renderRect = QRect(
        // QPoint(m_slot->bitmap_left, m_glyphMeta->bitmapDimension() - m_slot->bitmap_top),
        QPoint(m_slot->bitmap_left, m_slot->bitmap_top),
        QSize(m_bitmap.width, m_bitmap.rows));
}
