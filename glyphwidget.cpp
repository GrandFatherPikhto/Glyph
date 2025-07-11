#include "glyphwidget.h"

#define TRUNC(x) ((x) >> 6)

GlyphWidget::GlyphWidget(QWidget *parent)
    : QWidget{parent}
    , m_fontPath("C:/Windows/Fonts/Arial.ttf")
    , m_character(QChar('M'))
    , m_glyphImage(nullptr)
    , m_gridRows(8)
    , m_gridCols(8)
    , m_glyphRows(0)
    , m_glyphCols(0)
    , m_gridSize(11)
    , m_glyphSize(11)
{
    FT_Error ftErr = FT_Err_Ok;
    ftErr = FT_Init_FreeType(&m_ftLib);
    if (!ftErr) {
        ftErr = FT_New_Face(m_ftLib, m_fontPath.toLatin1().constData(), 0, &m_ftFace);
        if (!ftErr) {
            qDebug() << "Face Loaded";
        }
    }
}


int GlyphWidget::findOptimalGlyphSize ()
{
    if (m_ftFace == 0) {
        return -1;
    }

    int low = 1;
    int high = 1000;
    int bestSize = 0;
    while (low <= high) {
        int mid = (low + high) / 2;
        FT_Set_Pixel_Sizes(m_ftFace, 0, mid);
        FT_Load_Char(m_ftFace, m_character.unicode(), FT_LOAD_DEFAULT);

        // int glyphHeight = (m_ftFace->glyph->metrics.height >> 6);  // Переводим из 26.6 fixed-point в пиксели
        int glyphHeight = m_ftFace->glyph->bitmap.rows;
        // int glyphHeight = (m_ftFace->ascender - m_ftFace->descender) >> 6;
        // int glyphWidth = m_ftFace->glyph->metrics.width >> 6;  // Переводим из 26.6 fixed-point в пиксели

        // if (glyphHeight <= height() * 4 / 5)
        //     return mid;

        if (glyphHeight <= height() * 4 / 5) { // || glyphWidth <= maxWidth) {
            bestSize = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    qDebug() << "Best Size: " << bestSize << ", low: " << low << ", high: " << high << ", Height: " << height() * 4 / 5;

    return bestSize;
}


void GlyphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setRenderHint(QPainter::Antialiasing, false);

    int startX = 0;
    int startY = 0;


    // Горизонтальные линии
    for (int y = 0; y <= m_gridRows; ++y) {
        painter.drawLine(
            startX,
            startY + y * m_gridCellSize,
            startX + m_gridCols * m_gridCellSize,
            startY + y * m_gridCellSize
            );
    }

    // Вертикальные линии
    for (int x = 0; x <= m_gridCols; ++x) {
        painter.drawLine(
            startX + x * m_gridCellSize,
            startY,
            startX + x * m_gridCellSize,
            startY + m_gridRows * m_gridCellSize
            );
    }

    QImage glyphImage = m_glyphImage->scaled(m_gridCellSize * m_glyphCols, m_gridCellSize * m_glyphRows, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    if (m_glyphImage != nullptr)
    {
        painter.drawImage(
            // m_face->glyph->bitmap_left,  // X-позиция от начала
            0,
            // m_face->glyph->bitmap_top,  // Y-позиция от базовой линии
            0,
            glyphImage
            );
    }

    qDebug() << "Pixels Vector Size: " << m_glyphPixels.size() << ((m_glyphPixels.size() == m_glyphRows * m_glyphCols) ? "=" : "≠") << m_glyphRows * m_glyphCols;
    if (m_glyphPixels.size() == m_glyphRows * m_glyphCols) {
        for(qint32 y = 0; y < m_glyphRows; ++y) {
            for(qint32 x = 0; x < m_glyphCols; ++x) {
                // Получаем значение пикселя (true - закрашен, false - пустой)
                bool pixel = m_glyphPixels[y * m_glyphCols + x];

                // Рассчитываем координаты квадрата с учетом масштаба
                QRect pixelRect(
                    startX + x * m_gridCellSize,
                    startY + y * m_gridCellSize,
                    m_gridCellSize,
                    m_gridCellSize
                    );

                // Отрисовываем квадрат
                if(pixel) {
                    painter.fillRect(pixelRect, QColor(0x33, 0x33, 0x33, 0xEF));
                } else {
                    // painter.fillRect(pixelRect, Qt::white);
                    // painter.setPen(Qt::lightGray);
                    // painter.drawRect(pixelRect);
                }
            }
        }
    }

    painter.end();
}

void GlyphWidget::resetGlyphPixels ()
{

}

void GlyphWidget::renderGlyphPixels ()
{
    FT_Error ftError = FT_Err_Ok;
    resetGlyphPixels();

    if (m_ftFace == 0) {
        qDebug() << "Font Face Not inited";
        return;
    }

    FT_UInt glyph_index = 0;
    ftError = FT_Set_Pixel_Sizes(m_ftFace, 0, m_glyphSize);
    if (!ftError) {
        qDebug() << "Set pixel sizes to " << m_glyphSize;
    } else {
        qDebug() << "Can't set pixel sizes to " << m_glyphSize;
    }

    glyph_index = FT_Get_Char_Index(m_ftFace, m_character.unicode());

    ftError = FT_Load_Glyph(m_ftFace, glyph_index, FT_LOAD_RENDER | FT_LOAD_TARGET_MONO);

    if (!ftError)
    {
        FT_Pos left = m_ftFace->glyph->metrics.horiBearingX;
        FT_Pos right = left + m_ftFace->glyph->metrics.width;
        FT_Pos top = m_ftFace->glyph->metrics.horiBearingY;
        FT_Pos bottom = top - m_ftFace->glyph->metrics.height;
        // calcGlyphMetrics (m_glyphSize);

        m_glyphRect = QRect(QPoint(TRUNC(left),
                                   -TRUNC(top) + 1),
                            QSize(TRUNC(right - left) + 1,
                                  TRUNC(top - bottom) + 1));
        // setFixedSize(m_glyphRect.width(),
        //              m_glyphRect.height());
        qDebug() << "Loaded Glyph " << m_character << m_ftFace->glyph->bitmap.width << m_ftFace->glyph->bitmap.rows;

        FT_Bitmap* bitmap = &m_ftFace->glyph->bitmap;
        // qDebug() << "Columns: " << bitmap->width << ", Rows: " << bitmap->rows;
        m_glyphRows = bitmap->rows;
        m_glyphCols = bitmap->width;
        m_glyphPixels.clear();
        if (bitmap->pixel_mode == FT_PIXEL_MODE_MONO) {
            for (int y = 0; y < bitmap->rows; y++) {
                for (int x = 0; x < bitmap->width; x++) {
                    // Получаем байт, содержащий пиксель
                    unsigned char byte = bitmap->buffer[y * bitmap->pitch + (x / 8)];
                    // Извлекаем бит (MSB → x=0, LSB → x=7)
                    unsigned char bit = (byte >> (7 - (x % 8))) & 0x1;
                    // printf("%c", bit ? '#' : ' ');
                    m_glyphPixels.append(bit ? true : false);
                }
            }
        }
    } else {
        // qDebug() << "Error!";
    }
}

void GlyphWidget::renderGlyphImage()
{
    if (m_ftFace == 0) {
        return;
    }

    if (m_glyphImage) {
        delete m_glyphImage;
        m_glyphImage = nullptr;
    }

    int glyphSize = findOptimalGlyphSize();
    qDebug() << "Optimal Font Size: " << glyphSize << m_glyphCols * m_gridCellSize << "x" << m_glyphRows * m_gridCellSize;
    // Устанавливаем размер и загружаем глиф
    int glyphWidth = m_glyphCols * m_gridCellSize * 2;
    int glyphHeight = m_glyphRows * m_gridCellSize * 2;
    FT_Set_Pixel_Sizes(m_ftFace, glyphWidth, glyphHeight);

    FT_Load_Char(m_ftFace, m_character.unicode(), FT_LOAD_RENDER);

    FT_GlyphSlot glyph = m_ftFace->glyph;
    FT_Bitmap bitmap = glyph->bitmap;

    // Создаём QImage с форматом ARGB32 (для прозрачности)
    // QImage img(bitmap.width, bitmap.rows, QImage::Format_ARGB32);
    m_glyphImage = new QImage(bitmap.width, bitmap.rows, QImage::Format_ARGB32);
    m_glyphImage->fill(Qt::transparent);

    // Заполняем пиксели (для монохромного и grayscale-глифа)
    for (int y = 0; y < bitmap.rows; ++y) {
        for (int x = 0; x < bitmap.width; ++x) {
            unsigned char pixel;

            // Обрабатываем монохромный (1 бит на пиксель) и grayscale (8 бит)
            if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {
                pixel = (bitmap.buffer[y * bitmap.pitch + (x / 8)] >> (7 - (x % 8))) & 1 ? 255 : 0;
            } else { // FT_PIXEL_MODE_GRAY
                pixel = bitmap.buffer[y * bitmap.pitch + x];
            }

            // Устанавливаем пиксель в QImage (чёрный с альфа-каналом)
            m_glyphImage->setPixel(x, y, qRgba(0, 0, 0, pixel));
        }
    }
}

void GlyphWidget::updateGlyph () {
    m_gridCellSize = height() * 4 / (5 * m_gridRows);
    renderGlyphPixels();
    renderGlyphImage();
}

void GlyphWidget::setCharacter(const QChar &newCharacter) {
    m_character = newCharacter;
    if (m_character != QChar()) {
        updateGlyph();
        update();
    }
}

void GlyphWidget::setGlyphSize(int newFontSize) {
    m_glyphSize = newFontSize;
    updateGlyph();
    update();
}

void GlyphWidget::setGlyphFont(const QFont &newFont, const QString &newFontPath)
{
    m_font = newFont;
    updateGlyph();
    update();
}


void GlyphWidget::resizeEvent(QResizeEvent *event)
{
    updateGlyph();
    update();
}

void GlyphWidget::setGridSize(int newGlyphSize) {
    m_gridSize = newGlyphSize;
    m_gridCols = m_gridSize;
    m_gridRows = m_gridSize;
    updateGlyph();
    update();
}

void GlyphWidget::setGlyphParams(const QFont &newFont, const QString & newFontPath, int newFontSize, const QChar &newCharacter, int newGridSize)
{
    m_font = newFont;
    m_fontPath = newFontPath;
    m_glyphSize = newFontSize;
    m_character = newCharacter;
    m_gridSize = newGridSize;
    m_gridRows = newGridSize;
    m_gridCols = newGridSize;
    updateGlyph();
    update();
}
