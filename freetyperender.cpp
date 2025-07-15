#include "freetyperender.h"

FreeTypeRender::FreeTypeRender()
    : m_ftError(FT_Err_Ok)
    , m_ftLibrary(0)
    , m_ftFace(0)
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
    const QSize& targetSize,
    Qt::TransformationMode mode
) {
    m_glyphMeta = glyphMeta;
    // qDebug() << __FILE__ << __LINE__ << m_glyphMeta->toString();
    
    if (!m_glyphMeta->fontPath().isEmpty()) {
        loadFontFace();
    }

    setTargetSize(targetSize);
    loadGlyph();


    // Get the glyph slot
    FT_GlyphSlot slot = m_ftFace->glyph;
    FT_Bitmap bitmap = slot->bitmap;

    // qDebug() << __FILE__ << __LINE__ << bitmap.width << bitmap.rows;
    // Create a QImage from the FreeType bitmap
    QImage image;

    if (targetSize == QSize()) {
        image = QImage(bitmap.width, bitmap.rows, QImage::Format_ARGB32);
    } else {
        image = QImage(targetSize.width(), targetSize.height(), QImage::Format_ARGB32);
    }

    QRect glyphRect = QRect(
        QPoint(slot->bitmap_left, slot->bitmap_top - m_glyphMeta->gridSize()),
        QSize(bitmap.width, bitmap.rows));
    // qDebug() << __FILE__ << __LINE__ << glyphRect;
    glyphMeta->setGlyphRect(glyphRect);
    // qDebug() << __FILE__ << __LINE__ << glyphMeta->toString();

    for (int y = 0; y < bitmap.rows; ++y) {
        for (int x = 0; x < bitmap.width; ++x) {
            uint8_t pixel;
            // Обрабатываем монохромный (1 бит на пиксель) и grayscale (8 бит)
            if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {
                pixel = (bitmap.buffer[y * bitmap.pitch + (x / 8)] >> (7 - (x % 8))) & 1 ? 0xEF : 0;
            } else { // FT_PIXEL_MODE_GRAY
                pixel = bitmap.buffer[y * bitmap.pitch + x];
            }

            // Устанавливаем пиксель в QImage (чёрный с альфа-каналом)
            image.setPixel(x, y, qRgba(0xFF, 0x33, 0x33, pixel));
        }
    }

    // Scale the image if target size is specified and different from current size
    if (!targetSize.isEmpty() && image.size() != targetSize) {
        // image = image.scaled(targetSize, Qt::IgnoreAspectRatio, mode);
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

void FreeTypeRender::setTargetSize(const QSize &targetSize)
{
    if (targetSize != QSize())
    {
        m_ftError = FT_Set_Pixel_Sizes(m_ftFace, targetSize.width(), targetSize.height());
    } else
    {
        m_ftError = FT_Set_Pixel_Sizes(m_ftFace, 0, m_glyphMeta->glyphSize());
    }

    if (m_ftError)
    {
        throw std::runtime_error(QString("Can't set %1 to size %2").arg(m_glyphMeta->character()).arg(m_glyphMeta->glyphSize()).toStdString());
    }
}
